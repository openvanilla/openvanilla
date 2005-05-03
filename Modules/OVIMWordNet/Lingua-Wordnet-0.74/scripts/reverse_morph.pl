    #!/usr/bin/perl

    use Lingua::Wordnet;
    use strict;

    my $wn = new Lingua::Wordnet;
    my $word;
    my $pos;


    while (1) {
        print "Enter a word: ";
        chomp($word = <STDIN>);
        print "Enter a part-of-speech: ";
        chomp($pos  = <STDIN>);
        print $wn->reverse_morph($word,$pos) || "none found", "\n";
    }
