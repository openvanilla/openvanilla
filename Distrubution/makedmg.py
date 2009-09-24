#!/usr/bin/env python
# encoding: utf-8

import sys
import os
import shutil
import subprocess

version = "0.9"
current_folder = os.path.abspath(os.path.dirname(__file__))
disk = "OpenVanilla-" + version
pkg = disk + ".pkg"
dmg_icns = "../Artworks/icon2008/ovdmg/ovdmg.icns"

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
	project_target_name = "OpenVanilla (Loader IMK)"
	clean_command = "xcodebuild -project ../OpenVanilla.xcodeproj -configuration Release clean"
	build_command = "xcodebuild -project ../OpenVanilla.xcodeproj -configuration Release -target \"" + project_target_name + "\" build"
	# os.system(clean_command)
	os.system(build_command)
	
def copy_release_to_tmp():
	source_filename = "OpenVanilla.app"
	source_path = "../build/Release"
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

def clean_DS_store():
	command = "find /Library/OpenVanilla/ -name .DS_Store -exec rm -f {} ';'"
	os.system(command)

def build_package():
	package_maker = "/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker"
	arg = " --no-relocate -v -d ./OpenVanilla.pmdoc --version " + version + " --out " + pkg
	print package_maker + arg
	os.system(package_maker + arg)


def make_dmg():
	size = 6 * 1024 * 2;
	command = "/usr/bin/hdiutil"
	arg = [command, "create",  "-sectors", str(size), "-volname", disk, "-attach", "-fs", "HFS+", "OpenVanilla.dmg"]
	
	result = subprocess.Popen(arg, stdout=subprocess.PIPE).communicate()
	lines = result[0].split("\n")
	device = ""
	for line in lines:
		if line.find("Apple_HFS") > -1:
			space = line.find(" ")
			device = line[:12]
	shutil.copy(pkg, os.path.join("/Volumes", disk , pkg))
	shutil.copy(dmg_icns, os.path.join("/Volumes", disk , ".VolumeIcon.icns"))
	os.system("SetFile -a C " + os.path.join("/Volumes", disk))

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
	
	print "Building Package..."
	build_package()
	os.system("sudo rm -rf tmp")
	print "Done.."

	print "Building DMG..."
	make_dmg()
	os.system("sudo rm -rf " + pkg)
	os.system("sudo rm -rf OpenVanilla.dmg")
	print "Done.."
	pass


if __name__ == '__main__':
	main()

