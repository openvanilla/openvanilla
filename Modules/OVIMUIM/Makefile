BUILDER=xcodebuild
BUILDSTYLE?=Deployment
PROJECT=OVIMUIM.xcodeproj
DSTROOT=/

all:
	$(BUILDER) -project $(PROJECT) -configuration $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build
#	find . -name '*.mode1' -delete
#	find . -name '*.pbxuser' -delete

install:
	$(BUILDER) -project $(PROJECT) -configuration $(BUILDSTYLE) install DSTROOT=$(DSTROOT)

