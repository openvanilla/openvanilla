BUILDER=xcodebuild -project OVLoader.xcode
BUILDSTYLE?=Development
PROJECT=OVLoader.xcode

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE) install DSTROOT=/

