BUILDER=xcodebuild -project OVPrefPane.xcode
BUILDSTYLE?=Development
PROJECT=OVPrefPane.xcode

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	rm -rf /Library/PreferencePanes/OVPrefPane.prefPane
	mv build/OVPrefPane.prefPane /Library/PreferencePanes/.

