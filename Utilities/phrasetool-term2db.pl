#!/usr/bin/perl
print "begin;\n";
chomp(@x=<>);
while(@x) {
    if ($a=shift(@x)) {
        $b=shift @x;
        print "insert into phrase values(\"$a\", \"$b\", \"W3C glossary\", \"2006-01-11 15:43:00\", \"0\");\n";
    }
} 

print "commit;\n"

