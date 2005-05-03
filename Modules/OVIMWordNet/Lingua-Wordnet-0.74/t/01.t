# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

BEGIN { $| = 1; print "1..7\n"; }
END {print "not ok 1\n" unless $loaded;}
use Lingua::Wordnet;
$loaded = 1;
print "ok 1\n";

$wn = new Lingua::Wordnet;

my $synset = $wn->lookup_synset_offset("00447188%n");  # baseball

if ($synset) { print "ok 2\n"; }
else         { print "not ok 2\n"; }

my $words;
foreach $bb_synset ($synset->hyponyms) {
   $bb_synset->add_words("ballser");
   foreach $word ($bb_synset->words) {
      $words .= "$word, ";
   }
}
if ($words =~ /hardball/) {
    print "ok 3\n";
} else {
    print "not ok 3\n";
}

$words = "";
$synset2 = $wn->lookup_synset("travel","v",2);
foreach $word ($synset2->words) {
    $words .= "$word, ";
}
$words =~ s/\,\s*$//;
if ($words =~ /journey/) { print "ok 4\n"; }
else                     { print "not ok 4\n"; }

if ($wn->familiarity("boy","n") == 4) { print "ok 5\n"; }
else                                  { print "not ok 5\n"; }

if ($wn->morph("bluest","a") eq "blue") { print "ok 6\n"; }
else                                { print "not ok 6\n"; }

if ($synset =~ /baseball/) { print "ok 7\n"; }
else                        { print "not ok 7\n"; }                                 
$wn->close();


