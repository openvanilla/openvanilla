package OpenVanilla::Utility::Generator;

use strict;
use warnings;

use base 'OpenVanilla::Utility';

=head1 NAME

OpenVanilla::Utility::Generator - Generator base class

=cut

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
