package Lingua::Wordnet::Analysis;

use strict;
use Lingua::Wordnet;
use vars qw($VERSION);

$VERSION = '0.74';

=head1 NAME

Lingua::Wordnet::Analysis - Perl extension for high-level processing of Wordnet databases.  

=head1 SYNOPSIS

 use Lingua::Wordnet::Analysis;

 $analysis = new Lingua::Wordnet::Analysis;

 # How many articles of clothing have 'tongues'?
 $tongue = $wn->lookup_synset("tongue","n",2);
 @articles = $analysis->search($clothes,$tongue,"all_meronyms");
 
 # Are there any parts, of any kinds, of any shoes, made of glass?
 @shoe_types = $analysis->traverse("hyponyms",$shoes);
 $count = $analysis->search(@shoe_types,$glass,"stuff_meronyms");

 # Compute the intersection of two lists of synsets
 @array1 = $shoes->all_holonyms;
 @intersect = $analysis->intersection
       (\@{$shoes->attributes},\@{$socks->attributes});

 # Generate a list of the inherited comp_meronyms for "apple"
 @apple_hypernyms = $analysis->traverse("hypernyms",$apple);
 @apple_parts = $analysis->traverse("comp_meronyms",@apple_hypernyms);
 

=head1 DESCRIPTION

Lingua::Wordnet::Analysis supplies high-level functions for analysis of word relationships. Most of these functions process and return potentially large amounts of data, so only use them if you "know what you are doing."

These functions could have been put into Lingua::Wordnet::Synset objects, but I wanted to keep those limited to core functionality. Besides, many of these functions have unproven usefulness.

=head1 Lingua::Wordnet::Analysis functions

=item $analysis->match(SYNSET,ARRAY)

Finds any occurance of SYNSET in the synset list ARRAY and the list's pointers. Returns a positive value if a match is found. match() does not traverse.


=item $analysis->search(SYNSET1,SYNSET2,POINTER)

Searches all pointers of type POINTER in SYNSET1 for SYNSET2. search() is recursive, and will traverse all depths. Returns the number of matches.
    

=item $analysis->traverse(POINTER,SYNSET)

Traverses all pointer types of POINTER in SYNSET and returns a list of all synsets found in the tree.


=item $analysis->coordinates(SYNSET)

Returns a list of the coordinate sisters of SYNSET.


=item $analysis->union(LIST)

Returns a list of synsets which is the union of synsets LIST. The union consists of synsets which occur in any lists. This is useful, for example, for determining all the holonyms for two or more synsets.


=item $analysis->intersection(ref LIST)

Returns a list of synsets of the intersection of ARRAY1 list of synsets with ARRAY2 list of synsets. The intersection consists of synsets which occur in both lists. This is useful, for example, to determine which meronyms are shared by two synsets:

    @synsets = $analysis->intersection
        (\@{$synset1->all_meronyms},\@{$synset2->all_meronyms});

=item $analysis->distance(SYNSET1,SYNSET2,POINTER)

Returns an integer value representing the distance in pointers between SYNSET1 and SYNSET2 using POINTER as the search path.

=head1 EXAMPLES

To print out an inherited meronym list, use traverse():

    $orange = $wn->lookup_synset("orange","n",1);
    @orange_hypernyms = $analysis->traverse("hypernyms",$orange);
    foreach ($analysis->traverse("all_meronyms",@orange_hypernyms)) {
        print $_->words, "\n";
    }

Note that the inherited meronyms will not contain the direct meronyms of
$orange.


=head1 BUGS/TODO

There is tons that could go in this module ... submissions are welcome!

Lots of cleanup.

Need to add a search_path function that will return a path to a match as a linked list or hash of hashes.

Some might want inherited meronym/holonym trees.

Please send bugs and suggestions/requests to dbrian@brians.org. Development
on this module is active as of Winter 2000.

=head1 AUTHOR

Dan Brian <dbrian@brians.org>

=head1 SEE ALSO

Lingua::Wordnet.

=cut

sub new {
    my $class = shift;
    my $self = {};
    bless $self, $class;
    return $self;
}

sub match {
    my $self = shift;
    my $synset = shift;
    my @synsets = @_;
    my $match = 0;
    foreach (@synsets) {
        if ($_->{offset} eq $synset->{offset}) {
            $match++;
        }
    }
    return $match;
}

sub distance {
    my $self = shift;
    my $synset1 = shift;
    my $matching = shift;
    my $ptrtype = shift;
    my @synsets = ( );
    my $findit;
    $findit = sub {
        my $synset = shift;
        my $matching = shift;
        my $pointer = shift;
        my $count = shift;
        $count++;
        my @synsets1 = ( );
        my @list = ( );
        my $found = 0;
        eval("\@list = \$synset->$pointer");
        die ($@) if ($@);
        foreach (@list) {
            if ($_->{offset} eq $matching->{offset}) {
                return (1,$count);
            } 
            ($found,$count) = &{$findit}($_,$matching,$pointer,$count);
            if ($found) { return (1,$count); }
        }
        return (0,$count);
    };
    my ($found,$count) = &{$findit}($synset1,$matching,$ptrtype,0);
    if ($found) { return $count; }
    else { return 0; }
}


sub global_distance {
	my $self = shift;

	my $syns1 = shift;
	my $syns2 = shift;

	my $maxdist = shift; 
	my $dist = shift;
	my $checked = shift || {};

	my $parentpathes = shift;

	unless (defined $dist) {
		if ($#{$syns1} < $#{$syns2}) {
#			print "Swapping syns1 and syns2\n";
	    	my $tmp = $syns1;
    		$syns1 = $syns2;
    		$syns2 = $tmp;
		}
	}

	unless ($parentpathes) {
 		$parentpathes = [ (map { $_->offset } @{$syns2}) ];
	}
#	print "Checked " . scalar keys( %{$checked} ) . " so far.\n";
#	print "Now checking: " . scalar @{$syns2} . " syns agains " . scalar @{$syns1} . "\n";
	if ($maxdist && $dist && $dist > $maxdist) { return (undef, "Max. distance ($maxdist) reached\n");  }
	my @around;
	my @pathes;
	my $i = 0;
	foreach my $syn2 (@{$syns2}) {

		$checked->{$syn2->offset} = 1;
		foreach my $syn1(@{$syns1}) {
			# compare syn1 and syn2
			if ($self->equals($syn1, $syn2)) {
				return ( ( $dist || 0 ), ($parentpathes->[$i] || ""));
			}
# =comment (this was slower)
# 			if (my $hyper = is_hypernym($syn1, $syn2)) {
# 				return ($dist) ? ($dist+1, $parentpathes->[$i] . " : " . $hyper->offset . "|hyper") : (1, $parentpathes->[$i] . " : " . $hyper->offset . "|hyper");
# 			}
# 			if (my $hypo = is_hyponym($syn1, $syn2)) {
# 				return ($dist) ? ($dist+1, $parentpathes->[$i] . " : " . $hypo->offset . "|hypo") : (1, $parentpathes->[$i] . " : " . $hypo->offset . "|hypo");
# 			}
# 			if (my $mero = is_meronym($syn1, $syn2)) {
# 				return ($dist) ? ($dist+1, $parentpathes->[$i] . " : " . $mero->offset . "|mero") : (1, $parentpathes->[$i] . " : " . $mero->offset . "|mero");
# 			}
# 			if (my $holo = is_holonym($syn1, $syn2)) {
# 				return ($dist) ? ($dist+1, $parentpathes->[$i] . " : " . $holo->offset . "|holo") : (1, $parentpathes->[$i] . " : " . $holo->offset . "|holo");
# 			}
# =cut
		}
		# get surrounding synsets of $syn2
		my ($ar, $pa) = $self->get_surroundings($syn2, $checked, $parentpathes->[$i]);
		push @pathes, @{$pa};
		push @around, @{$ar};
		$i++;
	}

	return $self->global_distance($syns1, \@around, $maxdist, (($dist) ? $dist+1 : 1), $checked, \@pathes);
}


sub equals {
	my $self = shift;
	my $syn1 = shift;
	my $syn2 = shift;

	return 1 if ($syn1->offset eq $syn2->offset);
	return 0;
}

sub is_hypernym {
	my $self = shift;
	my $syn1 = shift;
	my $syn2 = shift;

	foreach ($syn2->hypernyms) {
		if ($self->equals($syn1, $_)) {
			# print "MATCH: $syn2 is a HYPERNYM of $syn1\n";
			return $_;
		}
	}
	return 0;
}

sub is_hyponym {
	my $self = shift;
	my $syn1 = shift;
	my $syn2 = shift;

	foreach ($syn2->hyponyms) {
		if ($self->equals($syn1, $_)) {
			# print "MATCH: $syn2 is a HYPONYM of $syn1\n";
			return $_;
		}
	}
	return 0;
}

sub is_meronym {
	my $self = shift;
	my $syn1 = shift;
	my $syn2 = shift;

	foreach ($syn2->all_meronyms) {
		if ($self->equals($syn1, $_)) {
			# print "MATCH: $syn2 is a MERONYM of $syn1\n";
			return $_;
		}
	}
	return 0;
}


sub is_holonym {
	my $self = shift;
	my $syn1 = shift;
	my $syn2 = shift;
	foreach ($syn2->all_holonyms) {
		if ($self->equals($syn1, $_)) {
			# print "MATCH: $syn2 is a HOLONYM of $syn1\n";
			return $_;
		}
	}
	return 0;
}


sub get_surroundings { 
	my $self = shift;
	my $syn = shift;
	my $checked = shift;
	my $parentpath = shift || "";
	my @around;
	my @pathes; 
	foreach ($syn->hypernyms) {
		unless ($checked->{$_->offset}) {
			push @pathes, join (" : ", $parentpath,  $_->offset . "|hyper");
			push @around,$_;
		}
	}
	foreach ($syn->hyponyms) {
		unless ($checked->{$_->offset}) {
			push @pathes, join (" : ", $parentpath,  $_->offset . "|hypo");
			push @around,$_;
		}
	}
	foreach ($syn->all_meronyms) {
		unless ($checked->{$_->offset}) {
			push @pathes, join (" : ", $parentpath, $_->offset . "|mero");
			push @around,$_;
		}
	}
	foreach ($syn->all_holonyms) {
		unless ($checked->{$_->offset}) {
			push @pathes, join (" : ", $parentpath, $_->offset . "|holo");
			push @around,$_;
		}
	}
	return (\@around, \@pathes);	
}

sub search {
    my $self = shift;
    my $synset1 = shift;
    my $matching = shift;
    my $ptrtype = shift;
    my $lastsynset;
    my @synsets;
    my $matchit;
    $matchit = sub {
        my $synset = shift;
        my @list;
        eval("\@list = \$synset->$ptrtype");
        die ($@) if ($@);
        foreach (@list) {
            if ($_->{offset} eq $matching->{offset}) {
                push (@synsets,$lastsynset);
            }
            $lastsynset = $_;
            &{$matchit}($_);
        }
    };
    &{$matchit}($synset1);
    @synsets;
}

sub traverse {
    my $self = shift;
    my $ptrtype = shift;
    my @synsets = ( );
    my %hash;
    my $traverseit;
    $traverseit = sub {
        my $synset = shift;
        my $pointer = shift;
        my @synsets1 = ( );
        my @list = ( );
        eval("\@list = \$synset->$pointer");
        die ($@) if ($@);
        foreach (@list) {
            unless (exists $hash{$_}) {
                push @synsets1, $_;
                push @synsets1, &{$traverseit}($_,$pointer);
                $hash{$_->{offset}} = "";
            }
        }
        @synsets1;
    };
    foreach (@_) {
        push @synsets, &{$traverseit}($_,$ptrtype);
    }
    @synsets;
}

sub coordinates {
    my $self = shift;
    my $synset = shift;
    return ($synset->hypernyms)[0]->hyponyms;
}

sub union {
    my $self = shift;
    my @synsets;
    my %union = ( );
    foreach (@_) {
        @union{$_->{offset}} = $_;
    }
    foreach (keys %union) {
        push(@synsets,$union{$_});
    }
    return @synsets;
}

sub intersection {
    my $self = shift;
    my ($i,$sizei) = (0, scalar @{$_[0]});
    my ($j,$sizej);
    my $set;
    for ($j = 1; $j < scalar @_; $j++) {
        $sizej = scalar @{$_[$j]};
        ($i,$sizei) = ($j,$sizej) if ($sizej < $sizei);
    }
    my @intersection;
    my $array = splice @_, $i, 1;
    my %valuehash;
    foreach (@$array) { push @intersection, 
        $_->{offset}; $valuehash{$_->{offset}} = $_; }
    while ($set = shift) {
        my $newlist;
        foreach (@$set) {
            my @offsets;
            push @offsets, $_->{offset};
            $newlist->{$_->{offset}} = "";
        }
        @intersection = grep { exists $newlist->{$_} } @intersection;
    }
    my @synsets;
    foreach (@intersection) { push @synsets, $valuehash{$_} }
    return @synsets;
}

sub close {
    my $self = shift;
    $self->DESTROY();
}

sub DESTROY {
    my $self = shift;
    undef $self;
}

1;
__END__
