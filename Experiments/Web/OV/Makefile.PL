use inc::Module::Install;

name 'OV';
all_from 'lib/OV.pm';

requires Catalyst => '5.62',
    'Catalyst::Model::DBIC' => 0,
    'Catalyst::View::JSON' => 0,
    ;

catalyst;

install_script glob('script/*.pl');
auto_install;
WriteAll;
