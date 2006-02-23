#!/bin/sh

cd PackageRoot

tar -cf - -T ../FileList --no-recursion | sudo tar -xpf -

sudo chown root:admin .

