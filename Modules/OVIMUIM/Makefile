BUILDER=xcodebuild
BUILDSTYLE?=Deployment
PROJECT=OVIMUIM.xcode
DSTROOT=/

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build
#	find . -name '*.mode1' -delete
#	find . -name '*.pbxuser' -delete

install:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT)

