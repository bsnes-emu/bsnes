ifeq ($(shell uname -s),Darwin)
DEFAULT := cocoa
else
DEFAULT := sdl
endif

VERSION := 0.5
default: $(DEFAULT)

ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS := $(DEFAULT)
endif


BIN := build/bin
OBJ := build/obj

CC := clang

CFLAGS += -Werror -Wall -std=gnu11 -ICore -D_GNU_SOURCE -DVERSION="$(VERSION)"
SDL_LDFLAGS := -lSDL
LDFLAGS += -lc -lm
CONF ?= debug

ifeq ($(shell uname -s),Darwin)
CFLAGS += -F/Library/Frameworks
OCFLAGS += -x objective-c -fobjc-arc -Wno-deprecated-declarations -isysroot $(shell xcode-select -p)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk -mmacosx-version-min=10.9
LDFLAGS += -framework AppKit -framework Carbon
SDL_LDFLAGS := -framework SDL
endif

ifeq ($(CONF),debug)
CFLAGS += -g
else ifeq ($(CONF), release)
CFLAGS += -O3
else
$(error Invalid value for CONF: $(CONF). Use "debug" or "release")
endif

cocoa: $(BIN)/Sameboy.app
sdl: $(BIN)/sdl/sameboy $(BIN)/sdl/dmg_boot.bin $(BIN)/sdl/cgb_boot.bin $(BIN)/sdl/LICENSE
bootroms: $(BIN)/BootROMs/cgb_boot.bin $(BIN)/BootROMs/dmg_boot.bin

CORE_SOURCES := $(shell echo Core/*.c)
SDL_SOURCES := $(shell echo SDL/*.c)

ifeq ($(shell uname -s),Darwin)
COCOA_SOURCES := $(shell echo Cocoa/*.m)
SDL_SOURCES += $(shell echo SDL/*.m)
endif

CORE_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(CORE_SOURCES))
COCOA_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(COCOA_SOURCES))
SDL_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(SDL_SOURCES))

ALL_OBJECTS := $(CORE_OBJECTS) $(COCOA_OBJECTS) $(SDL_OBJECTS)

# Automatic dependency generation
ifneq ($(MAKECMDGOALS),clean)
-include $(CORE_OBJECTS:.o=.dep)
ifneq ($(filter $(MAKECMDGOALS),sdl),)
-include $(SDL_OBJECTS:.o=.dep)
endif
ifneq ($(filter $(MAKECMDGOALS),cocoa),)
-include $(COCOA_OBJECTS:.o=.dep)
endif
endif

$(OBJ)/%.dep: %
	-@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MT $(OBJ)/$^.o -M $^ -c -o $@

$(OBJ)/%.c.o: %.c
	-@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJ)/%.m.o: %.m
	-@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OCFLAGS) -c $< -o $@

# Cocoa Port

Shaders:$(shell echo Shaders/*.fsh)

$(BIN)/Sameboy.app: $(BIN)/Sameboy.app/Contents/MacOS/Sameboy \
					$(shell echo Cocoa/*.icns) \
					Cocoa/License.html \
					Cocoa/info.plist \
					$(BIN)/BootROMs/dmg_boot.bin \
					$(BIN)/BootROMs/cgb_boot.bin \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/Document.nib \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/MainMenu.nib \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/Preferences.nib \
					Shaders
	mkdir -p $(BIN)/Sameboy.app/Contents/Resources
	cp Cocoa/*.icns $(BIN)/BootROMs/dmg_boot.bin $(BIN)/BootROMs/cgb_boot.bin $(BIN)/Sameboy.app/Contents/Resources/
	sed s/@VERSION/$(VERSION)/ < Cocoa/info.plist > $(BIN)/Sameboy.app/Contents/info.plist
	cp Cocoa/License.html $(BIN)/Sameboy.app/Contents/Resources/Credits.html
	mkdir -p $(BIN)/Sameboy.app/Contents/Resources/Shaders
	cp Shaders/*.fsh $(BIN)/Sameboy.app/Contents/Resources/Shaders

$(BIN)/Sameboy.app/Contents/MacOS/Sameboy: $(CORE_OBJECTS) $(COCOA_OBJECTS)
	-@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) -framework OpenGL -framework AudioUnit

$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/%.nib: Cocoa/%.xib
	ibtool --compile $@ $^
	
$(BIN)/sdl/sameboy: $(CORE_OBJECTS) $(SDL_OBJECTS)
	-@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) $(SDL_LDFLAGS)
	
$(BIN)/BootROMs/%.bin: BootROMs/%.asm
	-@mkdir -p $(dir $@)
	cd BootROMs && rgbasm -o ../$@.tmp ../$<
	rgblink -o $@.tmp2 $@.tmp
	head -c $(if $(filter dmg,$(CC)), 256, 2309) $@.tmp2 > $@
	@rm $@.tmp $@.tmp2

$(BIN)/sdl/%.bin: $(BIN)/BootROMs/%.bin
	-@mkdir -p $(dir $@)
	cp -f $^ $@
	
$(BIN)/sdl/LICENSE: LICENSE
	cp -f $^ $@

clean:
	rm -rf build
	