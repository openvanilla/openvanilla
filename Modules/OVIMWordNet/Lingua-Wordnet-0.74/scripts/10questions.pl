    #!/usr/bin/perl

    use Lingua::Wordnet;
    use Lingua::Wordnet::Analysis;
    use strict;

    my $wn = new Lingua::Wordnet;
    my $analysis = new Lingua::Wordnet::Analysis;

    my $password = $wn->lookup_synset("oven","n",1);
    my (@hypernyms,@attributes,@meronyms,@functions,$i);
    @hypernyms = $analysis->traverse("hypernyms",$password);
    for (@hypernyms) {
        push (@attributes,$_->attributes,$password->attributes);
        push (@meronyms,$_->all_meronyms,$password->all_meronyms);
        push (@functions,$_->functions,$password->functions);
    }
    my $i;

    for $i (1 .. 10) {
        print "$i> ";
        chomp ($_ = <STDIN>);
        if (/are\syou\san*\s(.+)\s*\?/i) {
            my $noun = $1; $noun =~ s/\s+$//; $noun =~ s/\s/\_/g;
            my @synsets = $wn->lookup_synset($noun,"n");
            my $found = 0; my $synset;
            foreach $synset (@synsets) {
                if ($synset->offset eq $password->offset) {
                    print " You guessed it!\n";
                    exit;
                }
                if ($analysis->match($synset,@hypernyms)) {
                    print " Yes.\n";
                    $found = 1;
                    last;
                }
            }
            print " No.\n" unless ($found);

        } elsif (/are\syou\s(.+)\s*\?/i) {
            my $adj = $1; $adj =~ s/\s+$//; $adj =~ s/\s/\_/g;
            my @synsets = $wn->lookup_synset($adj,"a");
            my $found = 0; my $synset;
            foreach $synset (@synsets) {
                if ($analysis->match($synset,@attributes)) {
                    print " Yes.\n";
                    $found = 1;
                    last;
                }
            }
            print " No.\n" unless ($found);

        } elsif (/do\syou\shave\s(?:an*\s)(.+)\s*\?/i) {
            my $noun = $1; $noun =~ s/\s+$//; $noun =~ s/\s/\_/g;
            my @synsets = $wn->lookup_synset($noun,"n");
            my $found = 0; my $synset; 
            foreach $synset (@synsets) {
                if ($analysis->match($synset,@meronyms)) {
                    print " Yes.\n";
                    $found = 1;
                    last;
                }
            }
            print " No.\n" unless ($found);

        } elsif (/(?:do|can)\syou\s(.+)\s*\?/i) {
            my $verb = $1; $verb =~ s/\s+$//; $verb =~ s/\s/\_/g;
            my @synsets = $wn->lookup_synset($verb,"v");
            my $found = 0; my $synset; 
            foreach $synset (@synsets) {
                if ($analysis->match($synset,@functions)) {
                    print " Yes.\n";
                    $found = 1;
                    last;
                }
            }
            print " No.\n" unless ($found);
        }
    }
    print " Sorry, you blew it.\n";

