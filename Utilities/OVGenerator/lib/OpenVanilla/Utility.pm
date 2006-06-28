package OpenVanilla::Utility;
use strict;
use warnings;

sub new {
    return bless {}, shift;
}

sub check_env {
    die "Please define OV_ROOT env var\n"
        unless defined $ENV{OV_ROOT};
    die "Please define OV_ROOT env var to be a directory.\n"
        unless -d $ENV{OV_ROOT};
}

1;
