#!/usr/bin/env python
# encoding: utf-8
"""
build_makelink.py

Created by zonble on 2009-09-07.
"""

import sys
import os

def build_debug():
	project_target_name = "OpenVanilla (Loader IMK)"
	clean_command = "xcodebuild -project ../OpenVanilla.xcodeproj -configuration Debug clean"
	build_command = "xcodebuild -project ../OpenVanilla.xcodeproj -configuration Debug -target \"" + project_target_name + "\" build"
	os.system(clean_command)
	os.system(build_command)

def logout():
	command = "/usr/bin/osascript -e 'tell application \"System Events\" to log out'"
	os.system(command)

def makelink():
	current_folder = os.path.abspath(os.path.dirname(__file__))
	debug_folder = os.path.join(current_folder, "../build/Debug/")
	home = os.environ['HOME']
	app = "OpenVanilla.app"
	target_folder = os.path.join(home, "Library/Input Methods")
	try:
		os.mkdir(target_folder)
	except Exception as e:
		print e
		pass
	
	source = os.path.join(debug_folder, app)
	target = os.path.join(target_folder, app)
	try:
		print "Creating symbolic link..."
		os.symlink(source, target)
		print "Done"
		logout()
	except Exception as e:
		print e
		pass
	

def main():
	build_debug()
	makelink()
	pass


if __name__ == '__main__':
	main()

