package Lingua::Wordnet;

require 5.005;
use strict;
use warnings;

use vars qw($VERSION @ISA @EXPORT_OK @EXPORT $DICTDIR $DELIM $SUBDELIM);
use DB_File;

require Exporter;

@ISA = qw(Exporter);
@EXPORT_OK = ( );
@EXPORT = qw( );
$VERSION = '0.74';
$DICTDIR = '/usr/local/WordNet-2.0/lingua-wordnet/';
$DELIM = '||';
$SUBDELIM = '|';

=head1 NAME

Lingua::Wordnet - Perl extension for accessing and manipulating Wordnet databases.

=head1 SYNOPSIS

  use Lingua::Wordnet;
  use Lingua::Wordnet::Analysis;

  $wn->unlock();
  $synset = $wn->lookup_synset("canary","n",4);
  $synset2 = $wn->lookup_synset("small","a",1);
  $synset->add_attributes($synset2);
  $synset->write();
  print $synset, "\n";
  $wn->close();

=head1 DESCRIPTION

Wordnet is a lexical reference system inspired by current psycholinguitics theories of human lexical memory. This module allows access to the Wordnet lexicon from Perl applications, as well as manipulation and extension of the lexicon. Lingua::Wordnet::Analysis provides numerous high-level extensions to the system.

Version 0.1 was a complete rewrite of the module in pure Perl, whereas the old module embedded the Wordnet C API functions. In order to use the module, the database files must first be converted to Berkeley DB files using the 'scripts/convertdb.pl' file. Why did I do that?

- The Wordnet API consists mostly of searching and text manipulation functions, something Perl is, um .. well suited for.

- Data retrieval is more fast with the hash lookup than with the binary searches 

- Converting the databases allows optional manipulation of the data, including adding and editing synsets, as well as extension of the system to allow for more pointer types (including noun attributes and 'functions').

 - Developers can use the Wordnet databases without needing to compile the Wordnet API and browsers, allowing Wordnet to run on any Perl/Berkeley DB-capable platform (the database files are still needed for the conversion, of course)

- A pure Perl implementation allows easier debugging and modification for people who want to experiment or alter the processing.


With that said, there are actually two modules. Lingua::Wordnet impersonates the basic Wordnet API functions for searching and retrieving data, as well as adding, editing, and deleting synsets. Lingua::Wordnet::Analysis brings the interface up a level, allowing commands like "is 'yellow' an attribute of any 'birds'", and taking care of the recursive analysis.


=head1 Lingua::Wordnet functions

=over 4

=item $wn = new Lingua::Wordnet( [DATA_DIR] );

Creates and assigns a new object of class Lingua::Wordnet. DATA_DIR is optional, and indicates the location of the index and data files. 


=item $wn->unlock()

Allows files to be written to when data is added/edited/deleted. 


=item $wn->lock()

Locks files to prohibit write permissions (default).


=item $wn->grep(TEXT)

Returns an array of compound words matching TEXT.


=item @synsets = $wn->lookup_synset( TEXT, POS [,NUMBER] )

Assigns a list of synset objects (Lingua::Wordnet::Synset) matching TEXT within POS, where POS is 'n', 'v', 'a', 's' or 'r'. Without NUMBER, lookup_synset() will return all matches in POS. NUMBER is the sequential order of the desired synset within POS.


=item $synset = $wn->lookup_synset_offset(SYNSET_OFFSET)

Assigns a synset object SYNSET_OFFSET.


=item $synset = $wn->new_synset(WORD,POS);

Creates a new (empty) synset entry in the database. Both WORD and POS are required. An offset will be assigned when write() is called.

=back

=head1 Lingua::Wordnet::Synset functions

=over 4

=item @words = $synset->words([TEXT ..)]

Retrieves or sets the list of words for this synset. add_words() should be used
if you are only adding an entry, rather than setting all entries. Each word is
in the format: TEXT%SENSE, where TEXT is the word, and SENSE is the sense number for the word. If SENSE is not supplied when assigning words to a synset, Lingua::Wordnet will assign the appropriate sense numbers to the words when $synset->write() is called (since they must be unique). In this case, the word list should consist only of the word text, without the '%'. The new words will be written to the data and index files.


=item $wn->familiarity(WORD, POS [, POLY_CNT])

Returns an integer of the familiarity/polysemy count for WORD in POS. Given a third value POLY_CNT, sets the polysemy count for WORD in POS. In Lingua::Wordnet, this is a value which must be updated by the user, and is not automatically modified. This makes it useful for recording familiarity or frequency counts outside of the Wordnet lexicons. Note that polysemy within Lingua::Wordnet can be identified for a given word by counting the synsets returned by lookup_synset().


=item $wn->morph(WORD, POS)

Returns an array containing the base form(s) of WORD in POS as found in the Wordnet morph files. The synset_lookup() functions performs morphological conversion automatically, so a call to morph() is not required. Changes: This fuynction now returns an array, because on WORD may have more than one base form.


=item $synset->overview()

Returns the terms and gloss for the synset in a format for printing. This method is also used to overload a print performed on the synset. Note that this is different from the "overview" parameter of the 'wn' executable, since it only returns information about the current synset.


=item $synset->write()

Writes any changes made to $synset to the database and updates all affected synset data and indexes. If $synset passes out of scope before write() is called, the changes are lost.


All of following functions retrieve data in synsets. Each has two corresponding
functions which can be called by prepending 'add_' or 'delete_' before the
function name. These functions accept a synset object or objects as input. Unless noted otherwise in the following functions, any returned data is a synset object or array of synset objects. See below for examples usages.

 $synset->antonyms()
 $synset->add_antonyms($synset2[, ...])
 $synset->delete_antonyms($synset2[, ...])

Returns, adds, or deletes antonyms for $synset. WARNING: When adding/deleting synset pointers to Wordnet, it is important to add pointer entries to the corresponding synset in order to maintain database accuracy. Earlier versions of this module planned to automate this function, however, they have been abandoned in favor of having control over database writes with the 'write()' function, and are now considered functionality which belongs outside of the module. Thus, your program must implement the functionality to, in the above examples, add an antonym entry to '$synset' for '$synset2', in addition to adding an antonym entry to '$synset2' for '$synset'.


=item $synset->hypernyms()

Returns hypernyms for $synset.

=item $synset->hyponyms()

Returns hyponyms for $synset.

=item $synset->entailment()

Returns verb entailment pointers.

=item $synset->synonyms()

Returns a list of words within $synset. 


=item $synset->comp_meronyms()

Returns component-object meronyms for $synset.


=item $synset->member_meronyms()

Returns member-collection meronyms for $synset.


=item $synset->stuff_meronyms()

Returns stuff-object meronyms for $synset (a.k.a. substance-object).


=item $synset->portion_meronyms()

Returns portion-mass meronyms for $synset.


=item $synset->feature_meronyms()

Returns feature-activity meronyms for $synset.


=item $synset->place_meronym()

Returns place-area meronyms for $synset.


=item $synset->phase_meronym()

Sets or returns phase-process meronyms for $synset.


=item $synset->all_meronyms()

Returns an array of synset objects for all meronyms types of $synset.


=item $synset->all_holonyms()

Returns an array of synset objects for all holonyms of $synset.


The following seven functions mirror the above functionality for holonyms, and accordingly have corresponding add_ and delete_ functions which update any set values to the corresponding meronym pointers:

=item $synset->comp_holonyms()

=item $synset->member_holonyms()

=item $synset->stuff_holonyms()

=item $synset->portion_holonyms()

=item $synset->feature_holonyms()

=item $synset->place_holonyms()

=item $synset->phase_holonyms()


=item $synset->gloss([TEXT])

Returns the glass for $synset. If TEXT is present, the gloss for $synset will
be assigned that value.


=item $synset->attributes()

Returns a list of synset objects of attribute pointers for $synset.


=item $synset->functions()

Returns a list of synset objects of function pointers for $synset.


=item $synset->causes()

Returns the 'cause to' pointers for verbs.


=item $synset->pertainyms()

Returns the 'pertains to' pointers for adj and adv.


=item $synset->frames()

Returns a text array of verb frames for $synset. The add_frames() and delete_frames() functions accept only integers corresponding to the frames. The list of frames can be edited in Wordnet.pm directly, but probably shouldn't be.


=item $synset->lex_info([INT])

Returns a string containing lexicographer file information. The optional INT assigns the lexicographer file information, and should correspond to the file list in Wordnet.pm.

=item $synset->offset()

Returns the synset offset of $synset.

=back 

=head1 EXAMPLES

Extensive examples can be found in the 'scripts/' directory; here I will
summarize the basic functionality. There are also some examples in the pod
documentation for Lingua::Wordnet::Analysis.

This will display a hypernym tree for $synset:

 my $synset = $wn->lookup_synset_offset("00333350%n"); 
 while ($synset = ($synset->hypernyms)[0]) {
    $i++;
    print " "x$i, "->", $synset->words, "\n";
 } 

Outputting the following for synset "baseball":

 -> field_game%0
  -> outdoor_game%0
   -> athletic_game%0
    -> sport%0athletics%0
     -> diversion%0recreation%0
      -> activity%0
       -> act%0human_action%0human_activity%0

The example below will create a synset object and print a list of the hyponyms 
for that object:
 
 use Lingua::Wordnet;
 my $wn = new Lingua::Wordnet;
 my $synset = $wn->lookup_synset("baseball","n",1);
 print "The following are kinds of baseball games:\n";
 foreach $bb_synset ($synset->hyponyms) {
     my $words;
     foreach $word ($bb_synset->words) {
         $word =~ s/\%\d+$//; $word =~ s/\_/ /g;
         $words .= "$word, ";
     }
     $words =~ s/\,\s*$//;
     print "  $words\n";
 }
 $wn->close();


This will output:


 The following are kinds of baseball games:
   professional baseball
   hardball
   perfect game
   no-hit game, no-hitter
   one-hitter, 1-hitter
   two-hitter, 2-hitter
   three-hitter, 3-hitter
   four-hitter, 4-hitter
   five-hitter, 5-hitter
   softball, softball game
   rounders
   stickball, stickball game


And an assignment example. This will create a new synset and add it to the kinds of baseball games. We unlock the Wordnet files to enable changes to the database:


 use Lingua::Wordnet;
 my $wn = new Lingua::Wordnet;
 $wn->unlock();
 my $synset = $wn->lookup_synset("baseball","n",1);
 my $newsynset = $wn->new_synset("fooball","n");
 $newsynset->gloss("A baseball game in which a foo is used.");
 $synset->add_hyponym($newsynset);
 $wn->close();


Remember, proceeded most synset functions with "add" will append the supplied data to the corresponding field, rather than replacing its value.


We could add an attribute 'fun' to "fooball" thus (not necessarily recommended pointer, but it will suffice for an example):


 $fun_synset = $wn->lookup_synset("fun","adj",1);
 $newsynset->add_attributes($fun_synset);


See the Lingua::Wordnet::Analysis documentation for examples to retrieving and searching entire trees and inheritance functions.

=head1 BUGS/TODO

Please send bugs and suggestions/requests to dbrian@brians.org. Development on this module is active as of Spring 2001.

Clean up code, put references where beneficial.

=head1 AUTHOR

Dan Brian <dbrian@brians.org>

=head1 SEE ALSO

Lingua::Wordnet::Analysis.

=cut

sub new {
    my $class = shift;
    my ($datapath) = shift || $DICTDIR;
    my $self = {};
    bless $self, $class;
    $self->{lock} = 1;
    $self->{indexobj} = tie %{$self->{indexhash}},
         "DB_File", "$datapath/lingua_wordnet.index", 
          O_RDWR, 0666, $DB_BTREE or
          die "Unable to load $datapath/lingua_wordnet.index: $!";
    $self->{dataobj} = tie %{$self->{datahash}},
         "DB_File", "$datapath/lingua_wordnet.data",
          O_RDWR, 0666, $DB_BTREE or
          die "Unable to load $datapath/lingua_wordnet.data: $!";
    $self->{morphobj} = tie %{$self->{morphhash}},
         "DB_File", "$datapath/lingua_wordnet.morph",
          O_RDWR, 0666, $DB_BTREE or
          die "Unable to load $datapath/lingua_wordnet.morph: $!";
    return $self;
}

sub lock {
    my $self = shift;
    $self->{lock} = 1;
}

sub unlock {
    my $self = shift;
    $self->{lock} = 0;
}

sub DESTROY {
    my $self = shift;
    untie $self->{dicthash};
    untie $self->{indexhash};
    untie $self->{morphhash};
    $self = {};
}

sub close {
    my $self = shift;
    $self->DESTROY();
}

sub familiarity {
    my $self = shift;
    my ($word,$pos) = @_;
	if ($self->{indexhash}->{"$word\%$pos"}) {
	    return (split(/\Q$DELIM/,$self->{indexhash}->{"$word\%$pos"}))[0];
	} else {
		return undef;
	}
}

sub lookup_synset {
    my $self = shift;
	my ($word,$pos,$num) = @_;	
    if (!exists($self->{indexhash}->{"$word\%$pos"})) {
		#print "Morphing $word\n";
        my @morphed = $self->morph($word,$pos);
		my @synsets;
		foreach (@morphed) {
			next if ($_ eq $word);
			#print "M: $_\n";
			push @synsets, $self->lookup_synset($_, $pos); 
		}
		if ($num) {
			return $synsets[$num-1];
		} else {
			return @synsets;
		}
    }
    if ($pos && $num) {
        my ($poly,$offsets) = split(/\Q$DELIM/,$self->{indexhash}->{"$word\%$pos"});
        my $offset = (split(/\Q$SUBDELIM/,$offsets))[$num-1] . "\%$pos";
        return Lingua::Wordnet::Synset->new(\$self,$offset,$pos);
    } else {
#		print qq|$word is in index: $self->{indexhash}->{"$word\%$pos"}\n|;
        my ($poly,$offsets) = split(/\Q$DELIM/,$self->{indexhash}->{"$word\%$pos"});
        my @offsets = (split(/\Q$SUBDELIM/,$offsets));
        my @synsets;
        foreach (@offsets) {
            push(@synsets,Lingua::Wordnet::Synset->new(\$self,"$_\%$pos",$pos)); 
        }
        return @synsets;
    }
}

sub lookup_synset_offset {
    my $self = shift;
    my $offset = shift;
    return Lingua::Wordnet::Synset->new(\$self,$offset);
}

sub morph {
    my $self = shift;
    my $word = shift;
    my $pos = shift;
    my $morphed = $self->{morphhash}->{"$word\%$pos"};
	return unless $morphed;
	my @morphed;
	if ($morphed =~ /\Q$DELIM/) {
		@morphed = split(/\Q$DELIM/, $morphed);
	} else {
		@morphed = ($morphed);
	}
	return wantarray ? @morphed : $morphed[0] || undef;
}

sub reverse_morph {
    my $self = shift;
    my $word = shift;
    my $pos  = shift;
    my %rev_hash = reverse %{$self->{morphhash}};
	if ($rev_hash{"$word"}) {
    	return $rev_hash{"$word"}; 
	} else {
		# this takes care of multiple baseforms of a word
		my @words = map { $rev_hash{$_} } grep {	/^$word\Q$DELIM/ || 
							/\Q$DELIM\E$word\Q$DELIM\E/ ||
							/\Q$DELIM\E$word$/ } keys %rev_hash;

		if ($#words > 0) {
			print STDERR "Warning Something is wrong: $word has more the one reverse morphed meanings\n";
			print STDERR "\t" . join(",", @words);
		}
		return $words[0];
	}
}

sub grep {
    my $self = shift;
    my $key = shift;
    if (!defined $key) { return; }
    my $origkey = $key;
    my $st = 0;
    my $value = 0;
    my @words;
    for ($st = $self->{indexobj}->seq($key, $value, R_CURSOR);
         $key =~ /^$origkey/i && $st == 0;
         $st = $self->{indexobj}->seq($key, $value, R_NEXT) )
      { push(@words, $key); }
    return @words;
}

sub new_synset {
    my $self = shift;
    my ($word,$pos) = @_;
    my $sense = 0;
    # assign a sense number
    if (exists($self->{indexhash}->{"$word\%$pos"})) {
        my ($poly,$offsets) = split(/\Q$DELIM/,$self->{indexhash}->{"$word\%$pos"});
        my @offsets = (split(/\Q$SUBDELIM/,$offsets));
        $sense = scalar(@offsets);
    }
    #$word = $word . "\%$sense";
    return Lingua::Wordnet::Synset->new(\$self,'',$pos,$word);
}

my @lexfiles = (
    "adj.all",
    "adj.pert",         
    "adv.all",          
    "noun.Tops",        
    "noun.act",         
    "noun.animal",      
    "noun.artifact",        
    "noun.attribute",       
    "noun.body",        
    "noun.cognition",       
    "noun.communication",   
    "noun.event",       
    "noun.feeling",     
    "noun.food",        
    "noun.group",       
    "noun.location",        
    "noun.motive",      
    "noun.object",      
    "noun.person",      
    "noun.phenomenon",      
    "noun.plant",       
    "noun.possession",      
    "noun.process",     
    "noun.quantity",        
    "noun.relation",        
    "noun.shape",       
    "noun.state",       
    "noun.substance",       
    "noun.time",        
    "verb.body",        
    "verb.change",      
    "verb.cognition",       
    "verb.communication",   
    "verb.competition",     
    "verb.consumption",     
    "verb.contact",     
    "verb.creation",        
    "verb.emotion",     
    "verb.motion",      
    "verb.perception",      
    "verb.possession",      
    "verb.social",      
    "verb.stative",     
    "verb.weather",     
    "adj.ppl"
);

my @vrbsents = (
    "",
    "Something ----s",
    "Somebody ----s",
    "It is ----ing",
    "Something is ----ing PP",
    "Something ----s something Adjective/Noun",
    "Something ----s Adjective/Noun",
    "Somebody ----s Adjective",
    "Somebody ----s something",
    "Somebody ----s somebody",
    "Something ----s somebody",
    "Something ----s something",
    "Something ----s to somebody",
    "Somebody ----s on something",
    "Somebody ----s somebody something",
    "Somebody ----s something to somebody",
    "Somebody ----s something from somebody",
    "Somebody ----s somebody with something",
    "Somebody ----s somebody of something",
    "Somebody ----s something on somebody",
    "Somebody ----s somebody PP",
    "Somebody ----s something PP",
    "Somebody ----s PP",
    "Somebody's (body part) ----s",
    "Somebody ----s somebody to INFINITIVE",
    "Somebody ----s somebody INFINITIVE",
    "Somebody ----s that CLAUSE",
    "Somebody ----s to somebody",
    "Somebody ----s to INFINITIVE",
    "Somebody ----s whether INFINITIVE",
    "Somebody ----s somebody into V-ing something",
    "Somebody ----s something with something",
    "Somebody ----s INFINITIVE",
    "Somebody ----s VERB-ing",
    "It ----s that CLAUSE",
    "Something ----s INFINITIVE",
);

package Lingua::Wordnet::Synset;

use overload qw("") => \&overview;

sub new {
    my $class = shift;
    my ($wn,$offset,$pos,$word) = @_;
    my $self = {};
    my $data;
    bless $self, $class;
    $self->{wn} = $wn;
    if ($offset) { 
        if (!exists(${$self->{wn}}->{datahash}->{$offset})) { 
            die "Synset $offset not found.";
        }
        $data = ${$self->{wn}}->{datahash}->{$offset};
        $self->{offset} = $offset;
        $self->{pos} = substr($offset,length($offset)-1);
        ($self->{filenum},$self->{words},$self->{ptrs},$self->{frames},
            $self->{gloss}) = split(/\Q$Lingua::Wordnet::DELIM/,$data);
    } else {
        $self->{offset} = "1\%$pos";
        $self->{pos} = $pos;
        $self->{words} = $word; 
        $self->{filenum} = "";
        $self->{ptrs} = "";
        $self->{frames} = "";
        $self->{gloss} = "";
    }
    #lex_filenum||words||pointers||frames||gloss
    return $self;
}

sub overview {
    my $self  = shift;
    my @wordlist = $self->words;
    my $words = join ", ", @wordlist;
    $words =~ s/\%\d//g;
    $words =~ s/\_/ /g;
    return "$words -- (" . $self->gloss . ")";
}

# write
sub write {
    my $self = shift;
    my $strippedoffset = "";
    if (${$self->{wn}}->{lock} == 1) {
        die "write() not allowed while Wordnet object is locked."
    } else {
        # ${$self->{wn}}->{datahash}->{"dgb3"} = 1;
        # check if this is a new synset, give it an offset if so
        if ($self->{offset} =~ /^1\%(\w)$/) {
            ${$self->{wn}}->{datahash}->{offsetcount}++;
            $strippedoffset = ${$self->{wn}}->{datahash}->{offsetcount};
            $self->{offset} = ${$self->{wn}}->{datahash}->{offsetcount} . "\%$1";
        }  else {
            $strippedoffset = $self->{offset};
            $strippedoffset =~ s/\%(\w)$//;
        }
        # write the data entry
        ${$self->{wn}}->{datahash}->{$self->{offset}} = 
            $self->{filenum} . "$Lingua::Wordnet::DELIM" . $self->{words} . "$Lingua::Wordnet::DELIM" .
            $self->{ptrs} . "$Lingua::Wordnet::DELIM" . $self->{frames} . "$Lingua::Wordnet::DELIM" .
            $self->{gloss}; 
        # write the index entries
        my $word;
        foreach $word ($self->words) {
            my $iword = $word . "\%" . $self->{pos};
            if (exists(${$self->{wn}}->{indexhash}->{$iword})) {
                # check if synset is already here
                unless (${$self->{wn}}->{indexhash}->{$iword} =~ 
                         /$strippedoffset/) {
                    ${$self->{wn}}->{indexhash}->{$iword} .= "$Lingua::Wordnet::SUBDELIM" .  
                        $strippedoffset;
                } 
            } else {
                ${$self->{wn}}->{indexhash}->{$iword} = "1$Lingua::Wordnet::DELIM" . $strippedoffset;
            }
        }
    }
    return $self->{offset};
}

# offset
sub offset {
    my $self = shift;
    return $self->{offset};
}

# pos
sub pos {
    my $self = shift;
    return $self->{pos};
}

# words
sub words {
    my $self = shift;
    my @newwords = shift;
    my @wordlist;
    if (@newwords > 0) {
        @wordlist = @newwords;
    }
    @wordlist = split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{words});
    return @wordlist;
}
sub add_words {
    my $self = shift;
    my @newwords = shift;
    if (@newwords == 0) { return; }
    my @wordlist = split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{words});
    push (@wordlist, @newwords);
    $self->{words} = join("$Lingua::Wordnet::SUBDELIM",@wordlist);
}
sub delete_words {
    my $self = shift;
    my @delwords = shift;
    my $word;
    if (@delwords == 0) { return; }
    my @wordlist = split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{words});
    my @retwords;
    foreach $word (@wordlist) {
        unless (grep {$word} @delwords) {
            push(@retwords,$word);
        }
    }
    $self->{words} = join("$Lingua::Wordnet::SUBDELIM",@retwords);
}

sub synonyms {
	my $self = shift;

	my @syns;
	my @words = $self->words;
	return wantarray ? @words : \@words;
}

# standard synset functions

sub synset_pointers {
    my ($self,$ptr) = @_;
    my @synsets = ();
    foreach (split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{ptrs})) {
        /^$ptr\w*\s(\d+)\%(\w)\s(\d{4})/ && do {
            push(@synsets,Lingua::Wordnet::Synset->new($self->{wn},"$1\%$2"));
        };
    }
    return @synsets;
}
sub add_synset_pointers {
    my ($self,$ptr,@synsets) = @_;
    my $synset;
    foreach $synset (@synsets) {
        if ($self->{ptrs} =~ /^$ptr\s$synset->{offset}/) {

        } else {
            $self->{ptrs} .= "$Lingua::Wordnet::SUBDELIM$ptr $synset->{offset} 0000";
        }
    }
}

sub delete_synset_pointers {
    my ($self,$ptr,@synsets) = @_;
    my $synset;
    my $delim = "$Lingua::Wordnet::SUBDELIM";
    foreach $synset (@synsets) {
        $self->{ptrs} =~ s/(?:\Q$delim\E)*$ptr\s$synset->{offset}\s\d{4}//g;
    }
    $self->{ptrs} =~ s/^\Q$Lingua::Wordnet::SUBDELIM//;
}

# antonyms: pointers are '!' (consistent)
sub antonyms {
    my $self = shift;
    return synset_pointers($self,"!");
}
sub add_antonyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"!",@add_synsets);
}
sub delete_antonyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"!",@delete_synsets);
}

# hypernyms: pointers are '@' (consistent)
sub hypernyms {
    my $self = shift;
    return synset_pointers($self,"\@");
}
sub add_hypernyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\@",@add_synsets);
}
sub delete_hypernyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\@",@delete_synsets);
}

# entailment: pointers are '*' (consistent)
sub entailment {
    my $self = shift;
    return synset_pointers($self,'\*');    
}
sub add_entailment {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\\*",@add_synsets);
}
sub delete_entailment {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'\*',@delete_synsets);
}

# hyponyms: pointers are '~' (consistent)
sub hyponyms {
    my $self = shift;
    return synset_pointers($self,"~");
}
sub add_hyponyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"~",@add_synsets);
}
sub delete_hyponyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"~",@delete_synsets);
}


# In case you're wondering why meronyms use "%" pointers
# while holonyms use "#" pointers, even though wninput.5 says
# that "#" is a meronym and "%" is a holonym ... I don't know.
# But the API uses IS..PTR as "#" and HAS..PTR as "%". If you 
# understand this, please let me know, because I haven't 
# figured it out. Logically is seems backwards to me. I think 
# it has to do with the "IS" vs. "HAS" language behind the 
# pointers. *shrug*

# meroynyms
sub all_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%");
}

# member meronym: pointers are '%m' (consistent)
sub member_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%m");
}
sub add_member_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%m",@add_synsets);
}
sub delete_member_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%m",@delete_synsets);
}

# stuff meronym: pointers are '%s' (consistent)
sub stuff_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%s");
}
sub add_stuff_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%s",@add_synsets);
}
sub delete_stuff_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%s",@delete_synsets);
}

# portion meronym: pointers are '%o' (new)
sub portion_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%o");
}
sub add_portion_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%o",@add_synsets);
}
sub delete_portion_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%o",@delete_synsets);
}

# component meronym: pointers are '%p' (consistent - part)
sub comp_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%p");
}
sub add_comp_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%p",@add_synsets);
}
sub delete_comp_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%p",@delete_synsets);
}

# feature meronym: pointers are '%f' (new)
sub feature_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%f");
}
sub add_feature_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%f",@add_synsets);
}
sub delete_feature_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%f",@delete_synsets);
}

# phase meronym: pointers are '%a' (new)
sub phase_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%a");
}
sub add_phase_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%a",@add_synsets);
}
sub delete_phase_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%a",@delete_synsets);
}

# place meronym: pointers are '%l' (new)
sub place_meronyms {
    my $self = shift;
    return synset_pointers($self,"\%l");
}
sub add_place_meronyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\%l",@add_synsets);
}
sub delete_place_meronyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\%l",@delete_synsets);
}


# holonyms
sub all_holonyms {
    my $self = shift;
    return synset_pointers($self,"#");
}

# member holonym: pointers are '#m' (consistent)
sub member_holonyms {
    my $self = shift;
    return synset_pointers($self,"#m");
}
sub add_member_holonyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#m",@add_synsets);
}
sub delete_member_holonyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#m",@delete_synsets);
}

# stuff holonym: pointers are '#s' (consistent)
sub stuff_holonyms {
    my $self = shift;
    return synset_pointers($self,"#s");
}
sub add_stuff_holonyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#s",@add_synsets);
}
sub delete_stuff_holonyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#s",@delete_synsets);
}

# portion holonym: pointers are '#o' (new)
sub portion_holonyms {
    my $self = shift;
    return synset_pointers($self,"#o");
}
sub add_portion_holonym {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#o",@add_synsets);
}
sub delete_portion_holonym {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#o",@delete_synsets);
}

# component holonym: pointers are '#p' (consistent - part)
sub comp_holonyms {
    my $self = shift;
    return synset_pointers($self,"#p");
}
sub add_comp_holonym {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#p",@add_synsets);
}
sub delete_comp_holonym {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#p",@delete_synsets);
}

# feature holonym: pointers are '#f' (new)
sub feature_holonyms {
    my $self = shift;
    return synset_pointers($self,"#f");
}
sub add_feature_holonym {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#f",@add_synsets);
}
sub delete_feature_holonym {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#f",@delete_synsets);
}

# phase holonym: pointers are '#a' (new)
sub phase_holonyms {
    my $self = shift;
    return synset_pointers($self,"#a");
}
sub add_phase_holonym {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#a",@add_synsets);
}
sub delete_phase_holonym {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#a",@delete_synsets);
}

# place holonym: pointers are '#l' (new)
sub place_holonyms {
    my $self = shift;
    return synset_pointers($self,"#l");
}
sub add_place_holonym {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"#l",@add_synsets);
}
sub delete_place_holonym {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"#l",@delete_synsets);
}

# cause: pointers are '>' (consistent)
sub causes {
    my $self = shift;
    return synset_pointers($self,"\>");
}
sub add_causes {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\>",@add_synsets);
}
sub delete_causes {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\>",@delete_synsets);
}

# verb group: pointers are '$' (consistent)
sub verb_group {
    my $self = shift;
    return synset_pointers($self,'\$');
}
sub add_verb_group {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\$",@add_synsets);
}
sub delete_verb_group {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'\$',@delete_synsets);
}

# similar to: pointers are '&' (consistent)
sub similar_to {
    my $self = shift;
    return synset_pointers($self,'\&');
}
sub add_similar_to {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\&",@add_synsets);
}
sub delete_similar_to {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'\&',@delete_synsets);
}

# participle of verb: pointers are '<' (consistent)
sub participles {
    my $self = shift;
    return synset_pointers($self,"\<");
}
sub add_participles {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\<",@add_synsets);
}
sub delete_participles {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\<",@delete_synsets);
}

# pertainym (pertains to noun): pointers are '\' (consistent)
sub pertainyms {
    my $self = shift;
    return synset_pointers($self,"\\");
}
sub add_pertainyms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\\",@add_synsets);
}
sub delete_pertainyms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\\",@delete_synsets);
}

# attribute: pointers are '=' (consistent, but now also nouns)
sub attributes {
    my $self = shift;
    return synset_pointers($self,"\=");
}
sub add_attributes {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\=",@add_synsets);
}
sub delete_attributes {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\=",@delete_synsets);
}

# derived from adjective: pointers are '\' (consistent)
sub derived_from_adj {
    my $self = shift;
    return synset_pointers($self,"\\");
}
sub add_derived_from_adj {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\\",@add_synsets);
}
sub delete_derived_from_adj {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,"\\",@delete_synsets);
}

# also see: pointers are '^' (consistent)
sub also_see {
    my $self = shift;
    return synset_pointers($self,'\^');
}
sub add_see_also {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"^",@add_synsets);
}
sub delete_see_also {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'\^',@delete_synsets);
}

# function: pointers are '+' (new)
# This could be expanded to include objects for the function:
# + verb_synset_offset pos src/trg [noun_synset_offset frame wordnum]
# Where the typical pointer syntax is followed by a list of [] enclosed 
# pointers to noun objects of the verb function, a frame number, and a 
# word number of the word in that frame which corresponds to the noun.
# But not now. :)

#sub functions {
#    my $self = shift;
#    return synset_pointers($self,'\+');
#}
#sub add_functions {
#    my $self = shift;
#    my @add_synsets = @_;
#    add_synset_pointers($self,"\+",@add_synsets);
#}
#sub delete_functions {
#    my $self = shift;
#    my @delete_synsets = @_;
#    delete_synset_pointers($self,'\+',@delete_synsets);
#}


# in WordNet 2.0 the '+' - pointer is used for lexical links for 
# derivational morphology (new)
#
# From CHANGES:
# Lexical links for derivational morphology are represented as 
# "+".  Note that in verson 1.7.1, some links between nouns and
# verbs were present in the lexicographer files, but not in the
# database files.  In the 1.7.1 lexicographer files, these
# links were represented by the "+" character, followed by a
# letter from "a" to "x".  The letter is no longer present, and
# all derivational links are indicated simply by the "+"
# pointer type.   Also note that the "+" character also
# precedes the list of verb frames in the verb data file.  The
# verb frame list always follows the pointer list, therefore
# the "+" character used for the different purposes are
# distinguishable.

sub derivedforms {
    my $self = shift;
    return synset_pointers($self,'\+');
}
sub add_derivedforms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"\+",@add_synsets);
}
sub delete_derivedforms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'\+',@delete_synsets);
}

# Domains and Domain Terms (new in WN 2.0)
#
# From the CHANGES:
# Some synsets have been organized into topical domains.
# Domains are always noun synset, however synsets from every
# syntacic category may be classified.  Each domain is further
# classified as a CATEGORY, REGION, or USAGE.  A pointer from a
# domain term synset to its topic is represented by the ";"
# pointer character, followed by "c", "r", or "u", indicating
# the type of domain.  The converse relation, from a domain to
# the synsets in that domain, is represented by the "-" pointer
# character, also followed by "c", "r", or "u".

sub all_domains {
    my $self = shift;
    return synset_pointers($self,";");
}

sub category_domains {
    my $self = shift;
    return synset_pointers($self,";c");
}
sub add_category_domains {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,";c",@add_synsets);
}
sub delete_category_domains {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,';c',@delete_synsets);
}

sub region_domains {
    my $self = shift;
    return synset_pointers($self,";r");
}
sub add_region_domains {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,";r",@add_synsets);
}
sub delete_region_domains {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,';r',@delete_synsets);
}

sub usage_domains {
    my $self = shift;
    return synset_pointers($self,";u");
}
sub add_usage_domains {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,";u",@add_synsets);
}
sub delete_usage_domains {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,';u',@delete_synsets);
}

# domain terms
sub all_domainterms {
	my $self = shift;
	return synset_pointers($self,"-");
}
sub category_domainterms {
	my $self = shift;
	return synset_pointers($self,"-c");
}
sub add_category_domainterms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"-c",@add_synsets);
}
sub delete_category_domainterms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'-c',@delete_synsets);
}

sub region_domainterms {
	my $self = shift;
	return synset_pointers($self,"-r");
}
sub add_region_domainterms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"-r",@add_synsets);
}
sub delete_region_domainterms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'-r',@delete_synsets);
}

sub usage_domainterms {
	my $self = shift;
	return synset_pointers($self,"-u");
}
sub add_usage_domainterms {
    my $self = shift;
    my @add_synsets = @_;
    add_synset_pointers($self,"-u",@add_synsets);
}
sub delete_usage_domainterms {
    my $self = shift;
    my @delete_synsets = @_;
    delete_synset_pointers($self,'-u',@delete_synsets);
}

sub lex_info {
    my $self = shift;
    return $lexfiles[$self->{filenum}];
}

sub frames {
    my $self = shift;
    my @frames = split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{frames});
    my @frametext;
    my $frame;
    foreach $frame (@frames) {
        my ($fnum,$wnum) = split(/ /,$frame);
        if ($wnum > 0) {
            my $wordtext = " (" . (split(/\Q$Lingua::Wordnet::SUBDELIM/,$self->{words}))[$wnum] . ")";
            push(@frametext,$vrbsents[$fnum] . $wordtext);
        } else {
            push(@frametext,$vrbsents[$fnum]);
        }
    }
    return @frametext;
}

sub gloss {
    my $self = shift;
    my $gloss = shift;
    if ($gloss) {
        $self->{gloss} = $gloss;
    }
    return $self->{gloss};
}

sub DESTROY {
    my $self = shift;
    $self = {};
}

1;
__END__

