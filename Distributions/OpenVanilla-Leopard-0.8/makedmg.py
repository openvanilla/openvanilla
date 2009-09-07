#!/usr/bin/env python
# encoding: utf-8
"""
makedmg.py

Created by zonble on 2009-09-07.
"""

import sys
import os
import shutil
import subprocess

version = "0.8.1"
current_folder = os.path.abspath(os.path.dirname(__file__))
disk = "OpenVanilla-" + version
pkg = disk + ".pkg"

applescript  = "tell application \"Finder\"\n\
		tell disk \"" + disk + "\"\n" + """
			open
			set theXOrigin to 40
			set theYOrigin to 60
			set theWidth to 450
			set theHeight to 368
			set theBottomRightX to (theXOrigin + theWidth)
			set theBottomRightY to (theYOrigin + theHeight)
            tell container window
                    set current view to icon view
                    set toolbar visible to false
                    set statusbar visible to false
                    set the bounds to {theXOrigin, theYOrigin, theBottomRightX, theBottomRightY}
                    set statusbar visible to false
            end tell

            set opts to the icon view options of container window
            tell opts
                    set icon size to 112
                    set arrangement to not arranged
            end tell
			update without registering applications
			delay 2
		end tell
	end tell
"""

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
	arg = " -d OpenVanilla.pmdoc -o " + pkg + " -t " + disk + " -e Resources"
	os.system(package_maker + arg)

def build_uninstaller():
	project_target_name = "OVUninstall"
	clean_command = "xcodebuild -project ../../Utilities/OVUninstall/OVUninstall.xcodeproj -configuration Release clean"
	build_command = "xcodebuild -project ../../Utilities/OVUninstall/OVUninstall.xcodeproj -configuration Release -target \"" + project_target_name + "\" build"
	os.system(build_command)

def make_dmg():
	size = 10 * 1024 * 2;
	command = "/usr/bin/hdiutil"
	# arg = "create -sectors " + str(size) + " -volname \"" + disk + "\" -attach -fs HFS+ \"OpenVanilla.dmg\""
	# os.system(command + " " + arg)
	# return 
	# cmd = command + " " + arg
	arg = [command, "create",  "-sectors", str(size), "-volname", disk, "-attach", "-fs", "HFS+", "OpenVanilla.dmg"]
	
	result = subprocess.Popen(arg, stdout=subprocess.PIPE).communicate()
	lines = result[0].split("\n")
	device = ""
	for line in lines:
		if line.find("Apple_HFS") > -1:
			space = line.find(" ")
			device = line[:12]
	shutil.copy2(pkg, os.path.join("/Volumes", disk , pkg))

	uninstaller = "Uninstall OpenVanilla.app"
	uninstaller_path = os.path.abspath(os.path.join("../../Utilities/OVUninstall/build/Release", uninstaller))
	shutil.copytree(uninstaller_path, os.path.join("/Volumes", disk , uninstaller))
	subprocess.Popen(["osascript", "-e", applescript])

	os.system("sleep 1")
	
	os.system("hdiutil detach " + device )
	command = "hdiutil convert OpenVanilla.dmg -format UDZO -imagekey zlib-level=9 -o " + disk + ".dmg"
	os.system(command)

def main():
	os.system("sudo rm -rf *.dmg")
	os.system("sudo rm -rf *.pkg")
	build_release()
	copy_release_to_tmp()
	build_loader_and_modules()
	clean_DS_store()
	os.chdir(current_folder)
	
	print "Building Package..."
	build_package()
	os.system("sudo rm -rf tmp")
	
	make_dmg()
	pass


if __name__ == '__main__':
	main()

