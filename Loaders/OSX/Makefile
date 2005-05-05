BUILDER=xcodebuild
BUILDSTYLE?=Deployment
PROJECT=OVLoaderOSX.xcode
DSTROOT=/

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	# mkdir -p ~/Library/OpenVanilla/0.7.0/
	$(BUILDER) -project $(PROJECT) -target OVInit -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT)
	$(BUILDER) -project $(PROJECT) -target OVLoader -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT)

