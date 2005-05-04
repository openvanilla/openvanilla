BUILDER=xcodebuild
BUILDSTYLE?=Deployment
PROJECT=OVSocketModule.xcode
DSTROOT=/

all:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	$(BUILDER) -project $(PROJECT) -buildstyle $(BUILDSTYLE) install DSTROOT=$(DSTROOT)