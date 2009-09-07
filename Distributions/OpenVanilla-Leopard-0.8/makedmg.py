#!/usr/bin/env python
# encoding: utf-8
"""
makedmg.py

Created by zonble on 2009-09-07.
"""

import sys
import os
import shutil

current_folder = os.path.abspath(os.path.dirname(__file__))

def build_release():
	project_target_name = "LeopardVanilla"
	clean_command = "xcodebuild -project ../../Loaders/OSX-Leopard/LeopardVanilla.xcodeproj -configuration Release clean"
	build_command = "xcodebuild -project ../../Loaders/OSX-Leopard/LeopardVanilla.xcodeproj -configuration Release -target \"" + project_target_name + "\" build"
	os.system(build_command)
	
def copy_release_to_tmp():
	source_filename = "OpenVanilla.app"
	source_path = "../../Loaders/OSX-Leopard/build/Release"
	source = os.path.abspath(os.path.join(current_folder, source_path, source_filename))
	target = os.path.abspath(os.path.join(current_folder, "tmp"))
	try:
		os.system("sudo rm -rf " + target)
	except Exception as e:
		print str(e)
		pass
	try:
		os.mkdir(target)
	except Exception as e:
		print str(e)
		pass
	shutil.copytree(source, os.path.join(target, source_filename))
	os.system("sudo chown -R root " + os.path.join(target, source_filename))
	
def build_loader_and_modules():
	os.chdir("../../")
	os.system("sudo make install")
	pass

def clean_DS_store():
	command = "find /Library/OpenVanilla/ -name .DS_Store -exec rm -f {} ';'"
	os.system(command)

def build_package():
	package_maker = "/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker"
	arg = " -d OpenVanilla.pmdoc -o \"OpenVanilla.pkg\" -t \"OpenVanilla\" -e Resources"
	os.system(package_maker + arg)

def main():
	build_release()
	copy_release_to_tmp()
	build_loader_and_modules()
	clean_DS_store()
	os.chdir(current_folder)

	print "Building Package..."
	build_package()
	pass


if __name__ == '__main__':
	main()

