OpenVanilla
===========

OpenVanilla is a cross-platform input method framework. This branch, called Oranje, focuses on providing a reference implementation based on OS X and a reference distribution.


Build and Installation
======================

1.  Build OpenVanilla.xcodeproj with Xcode
2.  Copy the built product, here we assume you're using the Release configuration, to the input method folder:

    *   For Mac OS X Leopard: Copy build/Release/OpenVanilla.app to ~/Library/Input Methods
    *   For Mac OS X Tiger: Copy build/Release/OpenVanilla.bundle to /Library/Components
    
Limitations
===========

1.  No Preferences app is provided. Edit the configuration in ~/Library/Preferences/org.openvanilla.plist. Changes take effect immediately.
2.  The candidate window is largely simplified. Only a minimal text label is provided.
3.  Only the core modules are provided and are currently bundled directly with the loader itself.
    *   OVIMArray
    *   OVIMEsperanto
    *   OVIMGeneric (when there is data)
    *   OVIMPhonetic
    *   OVIMTibetan
    *   OVIMUnicode    
4.  Output filters are not supported in this branch. Maybe not yet. Maybe for good.

On Mac OS X Tiger Support
=========================

This OpenVanilla implementation uses [IMK-Tiger](http://code.google.com/p/imk-tiger/) to provide Mac OS X Tiger support so that the IMK-based loader can be retrofitted into a text service manager bundle.

As Apple is releasing Mac OS X 10.6, it no longer makes sense to support Mac OS X Tiger. In this branch, Tiger-specific version (the product OpenVanilla.bundle) is still built, but there is no promise any future modification on the loader would maintain the build. No regression testing will be actively performed.
