BUILDER=xcodebuild -project OVLoader.xcode
BUILDSTYLE?=Development

all:
	$(BUILDER) -buildstyle $(BUILDSTYLE)

clean:
	$(BUILDER) clean
	rm -rf build

install:
	$(BUILDER) -buildstyle $(BUILDSTYLE) install DSTROOT=/

