package OV::Model::DBIC;

use strict;
use base 'Catalyst::Model::DBIC';

__PACKAGE__->config(
    dsn           => 'dbi:SQLite:root/imtables.db',
);

sub query {
    my ($self, $c) = @_;
    my $table = lc $c->req->args->[0];
    my $key = $c->req->args->[1];
    my @results = map { $_->value }
        $self->table($table)->search( 'key' => $key );
    $c->stash->{results} = \@results;
}

=head1 NAME

OV::Model::DBIC - DBIC Model Component

=head1 SYNOPSIS

See L<OV>

=head1 DESCRIPTION

DBIC Model Component.

=head1 AUTHOR

Liu Kang-min

=head1 LICENSE

This library is free software . You can redistribute it and/or modify
it under the same terms as perl itself.

=cut

1;
