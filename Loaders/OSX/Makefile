BUILDER=xcodebuild
BUILDSTYLE?=Deployment
PROJECT=OVLoaderOSX.xcodeproj
DSTROOT=/

all:
	$(BUILDER) -project $(PROJECT) -configuration $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build
#	find . -name '*.mode1' -delete
#	find . -name '*.pbxuser' -delete

install:
	# mkdir -p ~/Library/OpenVanilla/0.7.1/
	$(BUILDER) -project $(PROJECT) -target OVInit -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT) 
	$(BUILDER) -project $(PROJECT) -target OVLoader -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT) 
	$(BUILDER) -project $(PROJECT) -target OVPreferences -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT) 
	$(BUILDER) -project $(PROJECT) -target OVDisplayServer -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT) 

