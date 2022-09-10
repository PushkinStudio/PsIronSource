FRAMEWORKS := $(shell cat Source/PsIronSource/PsIronSource.Build.cs | grep "../../ThirdParty/iOS" | xargs -I{} basename {} ,)

all: $(FRAMEWORKS)

$(FRAMEWORKS): %: ThirdParty/iOS/%

ThirdParty/iOS/%: xcode-project/frameworks/% ThirdParty ThirdParty/iOS
	cp -R $< $@

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
