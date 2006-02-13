OpenVanilla 0.7.2 (beta) for OS X
Patch for a Bug in the Display Server

This package fixes a bug in both Array and Generic input method
modules, which did not properly process non-radical keys so that
they would not pass through the output filters.

To install, simply drag the two .dylib files into the following
folder:

    /Library/OpenVanilla/0.7.2/Modules
    
Note you'll be prompted for the system administrator privilege to 
overwrite the existing application.

After having replaced the application, you have to relogin for this
bug fix to take effect.

