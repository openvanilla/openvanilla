while (@ARGV) {
    parse(shift);
}


sub parse {
    $n=shift;
    print "$n";
    
    $l=0;
    $lngst=0;
    open F, "< $n";
    while (<F>) {
        if (/%chardef/ && /begin/) {
            while (<F>) {
                last if (/%chardef/);
                split;
                $lngst=length($_[0]) if (length($_[0]) > $lngst);
            }
        }
    }
    print " longest=$lngst\n";
}
