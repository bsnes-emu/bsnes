name := libretro.so
local := false
openmp := true
flags += -Wno-narrowing -Wno-multichar -g -fPIC

objects := libretro $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/libretro.o: target-libretro/libretro.cpp

all: $(objects)
ifeq ($(platform),linux)
	$(strip $(compiler) -o out/bsnes_libretro.so -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -Wl,-Bdynamic -lpthread -ldl -lgomp)
else ifeq ($(platform),windows)
	$(strip $(compiler) -o out/bsnes_libretro.dll -shared $(objects) -Wl,--no-undefined -Wl,--version-script=target-libretro/link.T -static-libgcc -static-libstdc++ -Wl,-Bstatic -lws2_32 -lpthread -lgomp -Wl,-Bdynamic)
else ifeq ($(platform),macos)
	$(strip $(compiler) -o out/bsnes_libretro.dylib -shared $(objects) -lpthread -ldl)
else ifeq ($(platform), ios-arm64)
    ifeq ($(IOSSDK),)
       IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
    endif
	$(strip c++ -arch arm64 -marm -miphoneos-version-min=11.0 -isysroot $(IOSSDK) -o out/bsnes_libretro_ios.dylib -shared $(objects) -lpthread -ldl)
else ifeq ($(platform), tvos-arm64)
    ifeq ($(IOSSDK),)
       IOSSDK := $(shell xcodebuild -version -sdk appletvos Path)
    endif
	$(strip c++ -arch arm64 -marm -mtvos-version-min=11.0 -isysroot $(IOSSDK) -o out/bsnes_libretro_tvos.dylib -shared $(objects) -lpthread -ldl)
endif
