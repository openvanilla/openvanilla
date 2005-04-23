#!/usr/bin/Perl -w -Tw
use strict;
use IO::Socket;
use Net::hostent;
use Encode;
use utf8;

# 20310 = 0x4f56 = 'OV'
my $client;
my $server = IO::Socket::INET->new (Proto=>'tcp', LocalPort=>20310, Listen=>SOMAXCONN, Reuse=>1);

die "cannot setup server" unless $server;

while ($client = $server->accept()) {
	print STDERR "OVSocketServer: Connected!\n";
	$client->autoflush(1);
	
	while(<$client>) {
        chomp;
        print STDERR "OVIMSocketServer: from client=$_\n";
        my @cmd=split (/\s/, decode("utf8", $_));        
        
        my $mod=shift @cmd;
        if ($mod =~ /ping/) {
            print $client "pong\n";
        }
        elsif  ($mod =~ /OVOFPerlTest/) {
            OVOFPerlTest(@cmd);
        } 
        elsif ($mod =~ /OVIMPerlTest/) {
            OVIMPerlTest(@cmd);
        }
	}

	close $client;
}	

print "OVSocketServer: Server closed\n";
close $server;


sub OVOFPerlTest {
    my $data=shift;
    my $src=shift;
    print STDERR "data=$data, src=$src\n";
    print $client $data+1, " ", encode("utf8", $src), "\n";
}

sub OVIMPerlTest {
    my $data=shift;
    my $code=shift;
    my $bufempty=shift;
    my $onscreen=shift;
    print STDERR "data=$data, code=$code, bufempty=$bufempty, onscreen=$onscreen\n";
    
    if ($code==97) {
        print $client "test bufappend aA bufupdate candiappend aA candiupdate candishow keyaccept\n";
    }
    elsif ($code==98) {
        print $client "test candiclear candihide bufsend keyaccept\n";
    } else {
        print $client "test keyreject\n";
    }
}
