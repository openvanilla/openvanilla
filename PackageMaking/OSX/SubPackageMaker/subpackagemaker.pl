#!/usr/bin/perl -w
use utf8;
use IO::All;

my $desctemplate < io "Description.plist.template";

my $listfile=shift or die "usage: subpackagemaker list\n";
open LIST, "<$listfile" or die "cannot open file $listfile\n";

while (<LIST>) {
    chomp;
    if (!/\@name/) { next; }
    
    my @d;
    @d = split /\s/;
    
    my $name=$d[1];
    
    my %title;
    my %desc;
    while (<LIST>) {
        chomp;
        if (/\@files/) { last; }
        if (/\@description/) { last; }
        if (!/\@title/) { next; }
        @d = split /\s/;
        if (scalar @d==1) {
            my $l=<LIST>;
            chomp $l;
            $title{"English"}=$l;
        }
        else {
            my $l=<LIST>;
            chomp $l;
            $title{$d[1]}=$l;
        }
    }
    
    if (/\@description/) {
        while (1) {
            if (/\@files/) { last; }
            print "desc $_\n";
            @d = split /\s/;
            if (scalar @d==1) {
                my $l=<LIST>;
                chomp $l;
                $desc{"English"}=$l;
            }
            else {
                my $l=<LIST>;
                chomp $l;
                $desc{$d[1]}=$l;
            }
            
            $_=<LIST>;
            chomp;
        }
        
    }
                            
    # encountered keyword @files
    my @files;        
    while (<LIST>) {
        chomp;
        if (/\@end/) { last; }
        
        push @files, $_;
        print "add file $_\n";
    }
        
    print "PARSED!\nname=$name\n";
    for my $x (keys %title) {
        print "$x=$title{$x}\n";
        
        my $outfile="Description.plist";
        
        if (!($x eq "default")) {
            system "mkdir -p Resource/$x.lproj";
            $outfile = "Resource/$x.lproj/" . $outfile;
            print "generating $outfile , locale=$x, content=$title{$x}!\n";
        }
        
        my $content = $desctemplate;
        $content =~ s/REPLACETITLE/$title{$x}/;
        if ($desc{$x})
            { $content =~ s/REPLACEDESCRIPTION/$desc{$x}/; }
        else
            { $content =~ s/REPLACEDESCRIPTION/$title{$x}/; }
        $content > io $outfile;
        
        if ($x eq "English") {
            $outfile="Description.plist";
            $content > io $outfile;
        }
    }
    
    open FLST, ">Filelist";
    for my $xx (@files) {
        print "file=$xx\n";
        print FLST "$xx\n";
    }
    close FLST;
    
    print "making package $name\n";
    system "./mksub.sh $name";
}

