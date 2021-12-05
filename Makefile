+.PHONY: all

all: release
install: install-release

ifdef ARCHS
BUILD_SETTINGS += ARCHS="$(ARCHS)"
BUILD_SETTINGS += ONLY_ACTIVE_ARCH=NO
endif

release: 
	xcodebuild -project OpenVanilla.xcodeproj -scheme OpenVanillaInstaller -configuration Release $(BUILD_SETTINGS) build

debug: 
	xcodebuild -project OpenVanilla.xcodeproj -scheme OpenVanilla -configuration Debug $(BUILD_SETTINGS) build
	xcodebuild -project OpenVanilla.xcodeproj -scheme OpenVanillaInstaller -configuration Debug $(BUILD_SETTINGS) build

DSTROOT = /Library/Input Methods
VC_APP_ROOT = $(DSTROOT)/OpenVanilla.app

.PHONY: permission-check install-debug install-release

permission-check:
	[ -w "$(DSTROOT)" ] && [ -w "$(VC_APP_ROOT)" ] || sudo chown -R ${USER} "$(DSTROOT)"

install-debug: permission-check
	rm -rf "$(VC_APP_ROOT)"
	open Build/Products/Debug/OpenVanillaInstaller.app

install-release: permission-check
	rm -rf "$(VC_APP_ROOT)"
	open Build/Products/Release/OpenVanillaInstaller.app

.PHONY: clean

clean:
	xcodebuild -scheme OpenVanilla -configuration Debug $(BUILD_SETTINGS)  clean
	xcodebuild -scheme OpenVanilla -configuration Debug $(BUILD_SETTINGS) clean
	xcodebuild -scheme OpenVanilla -configuration Release $(BUILD_SETTINGS)  clean
	xcodebuild -scheme OpenVanilla -configuration Release $(BUILD_SETTINGS) clean
	rm -f Source/Data/data.txt Source/Data/data-plain-bpmf.txt Source/Data/phrase.list Source/Data/PhraseFreq.txt
