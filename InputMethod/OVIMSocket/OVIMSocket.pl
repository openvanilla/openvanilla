#!/usr/bin/Perl -w -Tw

use strict;
use IO::Socket;
use Net::hostent;

my $server = IO::Socket::INET->new (Proto=>'tcp', LocalPort=>7986, 
    Listen=>SOMAXCONN, Reuse=>1);

die "cannot setup server" unless $server;

while (my $client = $server->accept())
{
	print STDERR "OVIMSocketServer: Connected!\n";
	$client->autoflush(1);
	
	while(<$client>)
	{
        chomp;
        print STDERR "OVIMSocketServer: from client=", $_, "\n";
        my @cmd=split /\s/;
        print STDERR "Key=$cmd[0], value=$cmd[1]\n";
        
        if ($cmd[0] eq "key")
        {
            my $c=chr $cmd[1];
            my $r;
            if ($c =~ /[0-9]/) { $r=$c.$c.$c; } else { $r=$c.$c; }
            print $client "buffer-add ", $r, "\n";
        }
	}

	close $client;
}	

print "Server closed\n";

close $server;