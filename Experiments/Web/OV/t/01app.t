use Test::More tests => 2;
BEGIN { use_ok( Catalyst::Test, 'OV' ); }

ok( request('/')->is_success );
