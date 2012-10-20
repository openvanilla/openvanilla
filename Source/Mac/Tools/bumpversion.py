#!/usr/bin/env python

import plistlib
import subprocess
import sys

# revs has an empty line, so len(rev) gives us what we want
# (which is commit count + 1)
revs = subprocess.check_output(["git", "rev-list", "HEAD"]).split("\n")

latest = None

for argv in sys.argv[1:]:
	plist = plistlib.readPlist(argv)
	print("updating: %s" % argv)

	bundle_version = int(plist["CFBundleVersion"])	
	short_version_components = plist["CFBundleShortVersionString"].split(".")
	print("was: %s, %s" % (bundle_version, short_version_components))

	bundle_version = str(len(revs))
	short_version_components[-1] = str(int(short_version_components[-1]) + 1)
	print("now: %s, %s" % (bundle_version, short_version_components))

	plist["CFBundleVersion"] = bundle_version
	plist["CFBundleShortVersionString"] = ".".join(short_version_components)
	latest = plist["CFBundleShortVersionString"]

	plistlib.writePlist(plist, argv)

if latest:
	msg = "Bump to %s" % latest
	cmd = ["git", "commit", "-m", msg] + sys.argv[1:]
	print cmd
	subprocess.call(cmd)
