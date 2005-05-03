#!/usr/bin/perl -w

##############################################################################
# 
# Changes by Koppi <koppi@iua.upf.es>, Sept. 2003
#
#	Code-Cleanup: added -w and use strict. Removed some global variables
#	and corrected some typos. Added some comments.
#  
#	Added variables $defaultdatadir and $defaultnewdir
#  
#	Loops over all <pos>es (%postypes)
#  
#	Morph-Files allow more than one base form of an inflected word!
#	This changed the structure of lingua_wordnet.morph, if there are more
#	than one baseform, the value of the morph-hash will contain both 
#	seperated by $DELIM
#
# TODO 
#	- use tagsensense_cnt in index
#	- syntactic markers
#	- which starting offset to use?
##############################################################################

use strict;

$|++;

use DB_File;
use Fcntl;

my $DELIM = '||';
my $SUBDELIM = '|';

my %sense_index = ();

# Syntactic Categories <pos>es
my %postypes = (
	"adj"	=> "a",
	"adv"	=> "r",
	"noun"	=> "n",
	"verb"	=> "v"
);

my $defaultdatadir = "/usr/local/WordNet-2.0/dict/";
my $defaultnewdir  = $defaultdatadir . "lingua-wordnet/";

my $datadir;
my $newdir;

#$DB_BTREE->{'flags'} = R_DUP;

print "This program will convert your Wordnet databases into files usable by
Lingua::Wordnet. This will not affect your existing Wordnet files, but
nwill require about 46MB disk space. Continue? [y]";

if (<STDIN> =~ /^n/i) { return; }
if ((scalar(@ARGV)) && shift(@ARGV) =~ /DICTDIR=(.*)\s*$/) {
    $datadir = $1;
}
if (!$datadir) {
    print "Data directory: [$defaultdatadir] ";
    $datadir = (<STDIN>);
    chop $datadir; 
    if (!$datadir) { $datadir = $defaultdatadir; }
}
if (!-e "$datadir/data.noun") {
    die "Directory $datadir doesn't exist, or doesn't contain the proper data files.";
}

print "Lingua::Wordnet needs to write the new files to a data directory.
Where do you want these files saved?
[$defaultnewdir] ";
$newdir = <STDIN>;
chop $newdir;
if (!$newdir) { $newdir = $defaultnewdir; }
if (!-d $newdir) {
    print "$newdir doesn't exist. Create it? [y] ";
    if (<STDIN> =~ /^n/i) { exit(0); }
    mkdir($newdir,0755) or do { print "Couldn't create $newdir: $!\n"; 
                                            exit(0); };
}
if (!-w $newdir) {
    die "$newdir doesn't exist, or you don't have permission to write there.";
}

print "\nStarting conversion of WordNet Files ...\n";

my %hash;


#############################################################
# create index hash of all lemmas in all Syntactic Categories
# key:		<lemma>%<pos>
# value: 	<poly_cnt>||<synset_offsets>
# 			synset_offsets = <offset>|<offset>|...

# remove existing old index
unlink "$newdir/lingua_wordnet.index";
# tie %hash to the DB_File lingua_wordnet.index
tie %hash, "DB_File", "$newdir/lingua_wordnet.index", O_RDWR|O_CREAT, 
			0666, $DB_BTREE
		    or die "Couldn't create $newdir/lingua_wordnet.index: $!" ;

print "Creating lingua_wordnet.index ...\n";
# lets do it for all index.<pos> files
foreach my $postype (keys %postypes) {
	printf ("\t%-20s => %s\n", "index.$postype", "lingua_wordnet.index");
	open FILE, "$datadir/index.$postype" 
			or die "Couldn't open $datadir/index.$postype $!";
	while (<FILE>) {
		chop $_;
		if (/^\s+/) { next; }
		my ($key,$value) = formatindex($_);
		$hash{$key} = $value;
	}
	close FILE;
}
# now we should have a hash containing all <lemma>%<pos> entries of 
# all index files
untie %hash;


#############################################################
# create the morph-file from all <pos>.exc files
# key:		<lemma>%<pos>
# value:	<word>[$DELIM<word>...]

# remove existing lingua_wordnet.morph
unlink "$newdir/lingua_wordnet.morph";

# tie lingua_wordnet.morph to %hash
tie %hash, "DB_File", "$newdir/lingua_wordnet.morph", O_RDWR|O_CREAT, 
			0666, $DB_BTREE
		    or die "Couldn't create lingua_wordnet.morph $!" ;

print "Creating lingua_wordnet.morph ...\n";
foreach my $postype (keys %postypes) {
	printf ("\t%-20s => %s\n", "$postype.exc ($postypes{$postype})", "lingua_wordnet.morph");
	open FILE, "$datadir/$postype.exc" 
			or die "Couldn't open $datadir/$postype.exc: $!";
	while (<FILE>) {
		my ($key,@values) = split(/\s/);
		# warn ("$key\%$postypes{$postype} already exists") if $hash{"$key\%$postypes{$postype}"};
		if ($hash{"$key\%$postypes{$postype}"}) {
			$hash{"$key\%$postypes{$postype}"} .= $DELIM . join($DELIM, @values);
		} else {
			$hash{"$key\%$postypes{$postype}"} = join($DELIM, @values);
		}
	}
	close FILE;
}

# and for backward compatibility let's do it for cousin.exc (if it exists)
if (-e "$datadir/cousin.exc") {
	printf ("\t%-20s => %s\n", "cousin.exc", "lingua_wordnet.morph");
	open FILE, "$datadir/cousin.exc" 
			or die "Couldn't open $datadir/cousin.exc: $!";
	while (<FILE>) {
		my ($key,@values) = split(/\s/);
		warn ("$key already exists") if $hash{"$key"};
		if ($hash{"$key"}) {
			$hash{"$key"} .= $DELIM . join($DELIM, @values);
		} else {
			$hash{"$key"} = join($DELIM, @values);
		}
	}
}
close FILE;

untie %hash;


#############################################################
# create the data-file from all data.<pos> files
# key:		<synset_offset>%<ss_type>
# value:	<lex_filenum>||<words>||<ptrs>||<frames>||<gloss>
#			<words>  = <word>%<sense_index>
#			<ptrs>   = <ptrsym> <ptroffset>%<ptrpos> <ptrsrc> ...
#			<frames> = <f_num> <w_num>[|<fnum> <w_num>...]

# remove existing lingua_wordnet.data file
unlink "$newdir/lingua_wordnet.data";

# tie lingua_wordnet.data to %hash
tie %hash, "DB_File", "$newdir/lingua_wordnet.data", O_RDWR|O_CREAT, 
			0666, $DB_BTREE
		    or die "Couldn't create lingua_wordnet.data: $!" ;

## why ???
$hash{offsetcount} = "10989500";

print "Creating lingua_wordnet.data ...\n";
foreach my $postype (keys %postypes) {
	printf ("\t%-20s => %s\n", "data.$postype", "lingua_wordnet.data");
	open FILE, "$datadir/data.$postype" 
			or die "Couldn't open $datadir/data.$postype $!";
	while (<FILE>) {
		chop $_;
		if (/^\s+/) { next; }
		my ($key,$data) = formatsynset($_,$postypes{$postype});
		$hash{$key} = $data;
	}
	close FILE;
}
untie %hash;

# print "  Gloss index:   \"index.gloss\" => \"lingua_wordnet.gloss\"\n";

print "\nConversion completed.\n"; sleep(1);

#if ($newdir ne "/usr/local/wordnet1.6/lingua-wordnet/"){
    chdir("../") if (`pwd` =~ /scripts\/*$/);
    link("Wordnet.pm","Wordnet.pm.old");
    unlink("Wordnet.pm");
    open FILE, "Wordnet.pm.old";
    open NEWFILE, ">Wordnet.pm";
    while (<FILE>) {
        if (/^\$DICTDIR/) {
            print NEWFILE "\$DICTDIR = '$newdir';\n";
        } else {
            print NEWFILE;
        }
    }
    close NEWFILE;
    close FILE;
#}

sub formatindex {
    my $string = shift;
    my $okparse = 1;
	
	# format of line, space seperated 
	# (see http://www.cogsci.princeton.edu/~wn/man1.7.1/wndb.5WN.html); 
	# lemma pos sense_cnt p_cnt [ptr_symbol...] sense_cnt tagsense_cnt synset_offset [synset_offset...] 
    # e.g. 's_gravenhage n 1 2 @ #p 1 0 06537474'
    
    my ($lemma,$pos,$poly_cnt,$p_cnt,$ptr_symbol,$sense_cnt,
		$tagsense_cnt,$synset_offsets) = ();

    my @synsets;
	
	#				lemma   pos  poly_cnt p_cnt (poly_cnt = sense_cnt)
    if ($string =~ /^(\S+)\s(\w)\s(\d+)\s(\d+)\s/g) {
        $lemma = $1;
        $pos = $2;
        $poly_cnt = $3;
        $p_cnt = $4;
#		print STDERR "\$p_cnt: $p_cnt\n";
        for (1 .. $p_cnt) { 
            # discard the pointer symbols
            # why ?? (koppi)
            if ($string !~ /\G(\S{1,2})\s/g) {
                print "pointers error $1\n";
                $okparse = 0;
            } else {
#				print STDERR "discarding: $1\n";
			}
        }

		# now the rest of the line should be: 
		# sense_cnt tagsense_cnt synset_offset [synset_offset ... ]	
        if ($string =~ /\G(\d+)\s(\d+)\s/g) {
            $sense_cnt = $1;
            $tagsense_cnt = $2; 
#			print STDERR "$lemma => tagsense_cnt: $tagsense_cnt\n" if ($tagsense_cnt);
        } else {
            print "sense_cnt error";
            $okparse = 0;
        }
		# get all synset_offset(s)
		# (= Byte offset in data.xxx file)
        for my $i (1 .. $sense_cnt) {
            if ($string =~ /\G(\d{8})\s*/g) {
                push(@synsets,$1);
				# set global sense index key to '<offset>%<pos>%<lemma>' and
				# value to $i (position)
                $sense_index{$1 . "%" . $pos . "%" . $lemma} = $i;
                #if ($lemma eq "plant") {
                #    print "sense_index: $1%$pos: $i\n";
                #}
            } else {
                print "synsets error\n";
                $okparse = 0;
            }
        }
    } else {
        print "whole error";
        $okparse = 0;
    }
    if ($okparse == 0) { 
        print "WARNING: index entry did not parse: $string\n";
    }
   
	# offsets of all synsets for this lemma 
    $synset_offsets = join($SUBDELIM,@synsets);
	# key : <lemma>%<pos>
    my $key = "$lemma\%$pos";
    #$poly_cnt||$synset_offsets
    my $data = "$poly_cnt$DELIM$synset_offsets";
    return ($key,$data);
}

sub formatsynset {
    my $string = shift;
    my $pos = shift;
    my $okparse = 1;

	# format of data.<pos>
	# synset_offset lex_filenum ss_type w_cnt word lex_id [word lex_id...] p_cnt [ptr...] [frames...] | gloss 
    my ($offset,$filenum,$ss_type,$w_cnt,$words,$ptrs,$frames,$gloss) = ();
    if ($string =~ /^(\d+)\s(\d{2})\s(\w)\s(\w{2})\s/g) {
        $offset = $1;
        $filenum = $2;
        $ss_type = $3;
        $w_cnt = $4;
        my $thiscount = 0;
        for (1 .. hex($w_cnt)) {
            if ($string =~ /\G(\S+)\s(\w)*\s*/g) {
                my $word = $1;
                my $lex_id = $2;
				my $syntactic_marker = "";
				if ($word =~ s/(\(\w{1,2}\))$//) {
					$syntactic_marker = $1;
#					print "Found syntactic marker: $syntactic_marker\n";
				}
                #if ($word eq "plant") {
#                    print "synset $offset%$pos:  " . $sense_index{$offset . "%" .  $pos . "%" . $word} . "\n";
				#}
                $words .= "$SUBDELIM$word\%" . $sense_index{$offset . "%" .
                        $pos . "%" . lc($word)};
            } else {
                $okparse = 0;
            }
            $words =~ s/^\Q$SUBDELIM//;
        }
		# now \G should be at: p_cnt [ptr...] [frames...] | gloss
        if ($string =~ /\G(\d{3})\s/g) {
            my $p_cnt = $1;
			if ($p_cnt != 0) { # avoid uninit. values
				for (1 .. $p_cnt) {
					my ($ptrsym,$ptroffset,$ptrpos,$ptrsrc) = "";
					if ($string =~ /\G(\S{1,2})\s(\d+)\s(\w)\s(\w{4})\s/g) {
						$ptrsym = $1;
						$ptroffset = $2; 
						$ptrpos = $3;
						$ptrsrc = $4;
						$ptrs .= "$SUBDELIM$ptrsym $ptroffset\%$ptrpos $ptrsrc";
					} else {
						$okparse = 0;
					}
				}
				$ptrs =~ s/^\Q$SUBDELIM// if $ptrs;
			} else {
				$ptrs = "";
			}
        } else {
            $okparse = 0;
        }
        if ($ss_type eq "v") {
            if ($string =~ /\G\s*(\d{2})\s/g) {
                my $f_cnt = $1;
                my ($f_num,$w_num) = ("","");
				$frames = "";
                for (1 .. $f_cnt) {
                    if ($string =~ /\G\+\s(\d{2})\s(\w{2})\s/g) {
                        $f_num = $1;
                        $w_num = $2; 
                        $frames .= "$SUBDELIM$f_num $w_num";
                    } else {
                        $okparse = 0;
                    }
                }
                $frames =~ s/^\Q$SUBDELIM// if $frames;
            } else {
                $okparse = 0;
            }
        } else {
			$frames = "";
		}
        if ($string =~ /\G\s*\|\s*(.+)\s*$/g) {
            $gloss = $1;
        } else {
			$gloss = "";
		}
    } else {
        $okparse = 0;
    }
    if ($okparse != 1) {
        print "WARNING: a synset did not parse properly:\n$string\n";
    }

    #$filenum||$pos||$ss_type||$words||$prts||$frames||$gloss
    if ($ss_type eq "s") { $ss_type = "a"; }
    my $key = "$offset\%$ss_type";
    my $data = "$filenum$DELIM$words$DELIM$ptrs$DELIM$frames$DELIM$gloss";
    return($key,$data);
}

