STATICLIBS := $(shell cat Source/PsIronSource/PsIronSource.Build.cs | grep "lib\S*.a" | xargs -I{} basename {})
FRAMEWORKS := $(shell cat Source/PsIronSource/PsIronSource.Build.cs | grep "../../ThirdParty/iOS" | xargs -I{} basename {} ,)

.PHONY: all
all: $(FRAMEWORKS)

.PHONY: SetStaticLibsPermissions
SetStaticLibsPermissions: $(STATICLIBS)
	chmod -R 700 ThirdParty/iOS/StaticLibs

$(STATICLIBS): %: ThirdParty/iOS/StaticLibs xcode-project/Pods
	$(eval LIBRARYFILE := $(shell find xcode-project/Pods -name "$(@)" | grep "ios-arm64_armv7"))
	$(eval LIBRARYDIR := $(shell echo $(LIBRARYFILE) | awk -F/ '{print $$(NF-3)}'))
	lipo $(LIBRARYFILE) -remove armv7 -output $(LIBRARYFILE)
	cp -R $(shell dirname ${LIBRARYFILE})/ $</$(LIBRARYDIR)

$(FRAMEWORKS): %: ThirdParty/iOS/%

ThirdParty/iOS/%: xcode-project/frameworks/% ThirdParty ThirdParty/iOS
	cp -R $< $@

ThirdParty/iOS/StaticLibs: ThirdParty ThirdParty/iOS
	mkdir ThirdParty/iOS/StaticLibs

ThirdParty/iOS: ThirdParty
	mkdir ThirdParty/iOS

ThirdParty:
	mkdir ThirdParty

.PRECIOUS: xcode-project/frameworks/%
xcode-project/frameworks/%: xcode-project/Pods
	cd xcode-project; ./update.sh # todo consider moving to Makefile

xcode-project/Pods:
	cd xcode-project; pod install --repo-update

.PHONY: clean
clean:
	rm -rf ThirdParty
	rm -rf xcode-project/downloads
	rm -rf xcode-project/frameworks
	rm -rf xcode-project/Pods
