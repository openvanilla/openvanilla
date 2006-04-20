use inc::Module::Install;

name 'OV';
all_from 'lib/OV.pm';

requires Catalyst => '5.62';

catalyst;

install_script glob('script/*.pl');
auto_install;
WriteAll;
