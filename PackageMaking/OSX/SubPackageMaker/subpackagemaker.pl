#!/usr/bin/perl -w
use utf8;
use IO::All;

my $infotemplate < io "Info.plist.template";
my $desctemplate < io "Description.plist.template";

my $listfile=shift or die "usage: subpackagemaker list\n";
open LIST, "<$listfile" or die "cannot open file $listfile\n";

print "start making packages. you may be asked for sudo password once.\n";

while (<LIST>) {
    chomp;
    if (!/\@name/) { next; }
    
    my @d;
    @d = split /\s/;
    
    my $name=$d[1];
    
    my $startparam="NoRestart";
    my $info=$infotemplate;
    if ($d[2]) { $startparam=$d[2]; }
    $info =~ s/REPLACESTARTPARAM/$startparam/;
    $info > io "Info.plist";    
    
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
    }
        
    print "start making package $name\n";
    for my $x (keys %title) {        
        my $outfile="Description.plist";
        
        if (!($x eq "default")) {
            system "mkdir -p Resource/$x.lproj";
            $outfile = "Resource/$x.lproj/" . $outfile;
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
        print FLST "$xx\n";
    }
    close FLST;
    
    system "./mksub.sh $name";
}

system "rm Info.plist";
system "rm Description.plist";

print "subpackagemaker terminates without error.\n";