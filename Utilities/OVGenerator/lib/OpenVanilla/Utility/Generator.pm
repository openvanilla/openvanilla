package OpenVanilla::Utility::Generator;

use strict;
use warnings;

=head1 NAME

OpenVanilla::Utility::Generator - Generator base class

=cut

sub new {
    my ($class) = @_;
    my $self = {};
    bless $self, $class;
    return $self;
}

sub load {
    my ($self, $class) = @_;
    my $subclass = __PACKAGE__ . "::\u${class}";
    eval "require $subclass";
    if ($@) {
        die "\u$class generator failed to load: $!\n";
    }
    return $subclass->new;
}

sub generate {
    die "Subclass must implement 'generate' method";
}


1;
