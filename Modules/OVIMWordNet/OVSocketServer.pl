#!/usr/bin/Perl -w -Tw
use strict;
use IO::Socket;
use Net::hostent;
use Encode;
use utf8;
use Lingua::Wordnet;

# 20310 = 0x4f56 = 'OV'
my $client;
my $server = IO::Socket::INET->new (Proto=>'tcp', LocalPort=>20310, Listen=>SOMAXCONN, Reuse=>1);

print "init wordnet...\n";
my $wn = new Lingua::Wordnet;
print "done!\n";

die "cannot setup server" unless $server;

# print STDERR "loading cj.cin.data\n";
# open (CJ, "<:utf8", "cj.cin.data");
# while (<CJ>) {
#    my @d=split;
#    # print "$d[0] => $d[1]\n";
#    $cjdata{$d[0]}.=encode("utf8", $d[1]);
# }
# close CJ;
# print STDERR "cj.cin.data loaded\n";

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
    $src="" if (!defined $src);
    print STDERR "data=$data, src=$src\n";
    $src =~ s/吃/喫/g;
    $src =~ s/才/纔/g;
    $src =~ s/只/祇/g;
    print $client $data+1, " \"", encode("utf8", uc $src), "\"\n";
}

sub OVIMPerlTest {
    my $raw=shift;
    print STDERR "raw data=$raw\n";
    my @dataarr = ($raw =~ /\"(.*)\"/);
    my $data=$dataarr[0];
    my $code=shift;
    my $bufempty=shift;
    my $onscreen=shift;
    
    $data =~ s/\\\"/\"/g;

    print STDERR "data=$data, code=$code, bufempty=$bufempty, onscreen=$onscreen\n";
    
    my $c=chr $code;
    if ($code==32) {
        if (!length($data)) {
            print $client "\"$data\" bufclear candiclear candihide keyreject\n";
        }
        else {
            my @synsetList;
            my @nounSynsets = $wn->lookup_synset($data, "n");
            foreach my $nounSynset (@nounSynsets) {
                push @synsetList, $nounSynset;
            }
            my @verbSynsets = $wn->lookup_synset($data, "v");
            foreach my $verbSynset (@verbSynsets) {
                push @synsetList, $verbSynset;
            }
            my @adjSynsets = $wn->lookup_synset($data, "a");
            foreach my $adjSynset (@adjSynsets) {
                push @synsetList, $adjSynset;
            }
            my @sSynsets = $wn->lookup_synset($data, "s");
            foreach my $sSynset (@sSynsets) {
                push @synsetList, $sSynset;
            }
            my @rSynsets = $wn->lookup_synset($data, "r");
            foreach my $rSynset (@rSynsets) {
                push @synsetList, $rSynset;
            }

            my $orig = $data;
            my $words;
            my %wordHash;
            my $freq = 1;
            foreach my $synset (@synsetList) {
                my @synonyms = $synset->synonyms(); 
                foreach my $word (@synonyms) {
                    $word =~ s/\%\d+$//;
                    $word =~ s/\_/ /g;
                    unless (exists $wordHash{$word})
                    {
                        $wordHash{$word} = $freq;
                        $freq++;
                    }
                }                
            }
            
            #simulate sorting by freq for the future.
            my @wordList =
                sort {$wordHash{$a} <=> $wordHash{$b}} keys %wordHash;
            my $counter = 1;
            foreach my $word (@wordList) {
                $words .= $counter . "." . $word . " ";
                $counter++;
                if($counter > 9) {
                    $counter = 1;
                    $words .= "\n";   
                }                
            }
            
            $data = "";            
            if(defined $words) {
                print $client "\"$data\" bufclear bufappend \"$orig \" bufsend bufclear candiclear candiappend \"$words\" candiupdate candishow keyaccept\n";
            }
            else {
                print $client "\"$data\" bufclear bufappend \"$orig \" bufsend bufclear candiclear candihide srvnotify \"no match!\" keyaccept\n";
            }               
        }
    }
    elsif ($c =~ /[[:print:]]/) {
        $data .= $c;
        print $client "\"$data\" bufappend \"$c\" bufupdate candiclear candihide keyaccept\n";
    }
    elsif ($code==13) {
        if (length $data) {
            print $client "\"\" candiclear candihide bufsend keyaccept\n";
        }
        else {
            print $client "\"$data\" keyreject\n";
        }
    } 
    elsif ($code==8) {
        if (length $data) {
            $data=substr($data, 0, -1);
            if (length $data) {
                print $client "\"$data\" bufclear bufappend \"$data\" bufupdate candiclear candiappend \"$data\" candiupdate keyaccept\n";
            }
            else {
                print $client "\"$data\" bufclear bufupdate candiclear candiupdate candihide keyaccept\n";
            }
        }
        else {
            print $client "\"$data\" candiclear candihide keyreject\n";
        }
    }
    else {
        print $client "\"$data\" keyreject\n";
    }
}