STATICLIBS := $(shell cat Source/PsIronSource/PsIronSource.Build.cs | grep "lib\S*.a" | xargs -I{} basename {})
FRAMEWORKS := $(shell cat Source/PsIronSource/PsIronSource.Build.cs | grep "../../ThirdParty/iOS" | xargs -I{} basename {} ,)

all: $(FRAMEWORKS) SetStaticLibsPermissions

.PHONY: SetStaticLibsPermissions
SetStaticLibsPermissions: $(STATICLIBS)
	chmod -R 700 ThirdParty/iOS/StaticLibs

$(STATICLIBS): %: ThirdParty/iOS/StaticLibs xcode-project/Pods 
	cp -R $(shell find xcode-project/Pods -name "$(@)" | xargs dirname) $<

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
	cd xcode-project; pod install

.PHONY: clean
clean:
	rm -rf ThirdParty
	rm -rf xcode-project/downloads
	rm -rf xcode-project/frameworks
