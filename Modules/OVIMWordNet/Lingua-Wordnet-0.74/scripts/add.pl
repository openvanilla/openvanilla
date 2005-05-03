use Lingua::Wordnet;
my $wn = new Lingua::Wordnet;

$wn->unlock();

my $synset = $wn->lookup_synset("baseball","n",1);
my $newsynset = $wn->new_synset("changelini","n");
$newsynset->gloss("A baseball game in which a foo is used.");
print $newsynset->write;
#$synset->add_hyponym($newsynset);

print $wn->lookup_synset("changelini","n",1);

$wn->close();

