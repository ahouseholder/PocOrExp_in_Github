source: http://www.securityfocus.com/bid/7433/info

Libopt library has been reported prone to a buffer overflow vulnerability.

It has been reported that several Libopt.a error logging functions, may be prone to buffer overflow vulnerabilities when handling excessive data. The data may be supplied as an argument to a program linked to the vulnerable library. This condition arises from a lack of sufficient bounds checking performed on the user-supplied data, before it is copied into a memory buffer. As a result the bounds of an internal stack-based memory buffer may be overflowed and adjacent memory corrupted with attacker supplied data. It should be noted that no SUID applications linked to this library are currently known.

Although unconfirmed this vulnerability may be exploited to execute arbitrary attacker supplied code.

It should be noted that although this vulnerability was reported to affect Libopt.a version 3.18 previous versions might also be affected. 

/* To compile vuln.c :                              */
/* cc -o vuln vuln.c /path/to/opt-3.18/src/libopt.a */

main(int *argc, char **argv)
{
  /* use OPT opt_atoi() */
        int y = opt_atoi(argv[1]);        printf("opt_atoi(): %i\n", y);
}