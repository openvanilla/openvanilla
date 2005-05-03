# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'
 
BEGIN { $| = 1; print "1..8\n"; }
END {print "not ok 1\n" unless $loaded;}
use Lingua::Wordnet::Analysis;
$loaded = 1;
print "ok 1\n";


$wn = new Lingua::Wordnet($ARGV[0]);
$analysis = new Lingua::Wordnet::Analysis;

$synset = $wn->lookup_synset_offset("00447188%n"); # baseball
$synset2 = $wn->lookup_synset("ball_game","n",1);

if ($analysis->match($synset2,$synset->hypernyms) > 0)
     { print "ok 2\n"; }
else { print "not ok 2\n"; }

$synset = $wn->lookup_synset("tree","n",1);
$synset2 = $wn->lookup_synset("bark","n",1);

if ($analysis->search($synset,$synset2,"comp_meronyms"))
     { print "ok 3\n"; }
else { print "not ok 3\n"; }

if ($analysis->coordinates($synset)) 
     { print "ok 4\n"; }
else { print "not ok 4\n"; }

$synset = $wn->lookup_synset("apple","n",1);
if ($analysis->union($synset->all_meronyms,$synset->hyponyms))
     { print "ok 5\n"; }
else { print "not ok 5\n"; }

$synset = $wn->lookup_synset("leg","n",1);
$synset2 = $wn->lookup_synset("human","n",2);
@array1 = $synset->comp_meronyms;
@array2 = $synset2->comp_meronyms;
if ($analysis->intersection(\@array1,\@array2))
     { print "ok 6\n"; }
else { print "not ok 6\n"; } 

$orange = $wn->lookup_synset("orange","n",3);
@orange_hypernyms = $analysis->traverse("hypernyms",$orange);
if ($analysis->traverse("comp_meronyms",@orange_hypernyms))
     { print "ok 7\n"; }
else { print "not ok 7\n"; }

if ($analysis->distance($orange,$orange_hypernyms[3],"hypernyms") == 4)
     { print "ok 8\n"; }
else { print "not ok 8\n"; }

