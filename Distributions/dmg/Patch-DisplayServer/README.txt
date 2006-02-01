OpenVanilla 0.7.2 (beta) for OS X
Patch for a Bug in the Display Server

This package fixes a bug in the display server (OVDisplayServer.app),
which caused the "candidate bar" to be positioned in the wrong place
when the user was in dual-screen mode and used an arrangement different
from the system default (where the top edges of the screens are
aligned together).

To install, simply drag the "OpenVanilla Display Server" application
into the following folder:

    /Library/OpenVanilla/0.7.2/Loader/
    
Note you'll be prompted for the system administrator privilege to 
overwrite the existing application.

After having replaced the application, you have to relogin for this
bug fix to take effect.

