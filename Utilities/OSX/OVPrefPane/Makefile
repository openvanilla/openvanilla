BUILDER=xcodebuild -project OVPrefPane.xcode
BUILDSTYLE?=Development
PROJECT=OVPrefPane.xcode

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	mv build/OVPrefPane.prefPane /Library/PreferencePanes/.

