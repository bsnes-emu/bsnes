# Set target, configuration, version and destination folders

PLATFORM := $(shell uname -s)

ifeq ($(PLATFORM),Darwin)
DEFAULT := cocoa
else
DEFAULT := sdl
endif

default: $(DEFAULT)

ifeq ($(MAKECMDGOALS),)
MAKECMDGOALS := $(DEFAULT)
endif

VERSION := 0.6
CONF ?= debug

BIN := build/bin
OBJ := build/obj

# Set tools

CC := clang
ifeq ($(PLATFORM),windows32)
# To force use of the Unix version instead of the Windows version
MKDIR := $(shell which mkdir)
else
MKDIR := mkdir
endif

# Set compilation and linkage flags based on target, platform and configuration

CFLAGS += -Werror -Wall -std=gnu11 -ICore -D_GNU_SOURCE -DVERSION="$(VERSION)" -I. -D_USE_MATH_DEFINES
SDL_LDFLAGS := -lSDL
ifeq ($(PLATFORM),windows32)
CFLAGS += -IWindows
LDFLAGS += -lmsvcrt -lSDLmain -Wl,/MANIFESTFILE:NUL
else
LDFLAGS += -lc -lm
endif

ifeq ($(PLATFORM),Darwin)
CFLAGS += -F/Library/Frameworks
OCFLAGS += -x objective-c -fobjc-arc -Wno-deprecated-declarations -isysroot $(shell xcode-select -p)/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk -mmacosx-version-min=10.9
LDFLAGS += -framework AppKit -framework Carbon
SDL_LDFLAGS := -framework SDL
endif

ifeq ($(PLATFORM),windows32)
CFLAGS += -Wno-deprecated-declarations # Seems like Microsoft deprecated every single LIBC function
LDFLAGS += -Wl,/NODEFAULTLIB:libcmt
endif

ifeq ($(CONF),debug)
CFLAGS += -g
ifeq ($(PLATFORM),windows32)
LDFLAGS += -Wl,/debug
endif
else ifeq ($(CONF), release)
CFLAGS += -O3 -DNDEBUG
ifneq ($(PLATFORM),windows32)
LDFLAGS += -flto
CFLAGS += -flto
endif
else
$(error Invalid value for CONF: $(CONF). Use "debug" or "release")
endif

# Define our targets

ifeq ($(PLATFORM),windows32)
SDL_TARGET := $(BIN)/sdl/sameboy.exe $(BIN)/sdl/sameboy_debugger.exe $(BIN)/sdl/SDL.dll
else
SDL_TARGET := $(BIN)/sdl/sameboy
endif

cocoa: $(BIN)/Sameboy.app
sdl: $(SDL_TARGET) $(BIN)/sdl/dmg_boot.bin $(BIN)/sdl/cgb_boot.bin $(BIN)/sdl/LICENSE
bootroms: $(BIN)/BootROMs/cgb_boot.bin $(BIN)/BootROMs/dmg_boot.bin

# Get a list of our source files and their respective object file targets

CORE_SOURCES := $(shell ls Core/*.c)
SDL_SOURCES := $(shell ls SDL/*.c)

ifeq ($(PLATFORM),Darwin)
COCOA_SOURCES := $(shell ls Cocoa/*.m) $(shell ls HexFiend/*.m)
SDL_SOURCES += $(shell ls SDL/*.m)
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
	-@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -MT $(OBJ)/$^.o -M $^ -c -o $@

# Compilation rules

$(OBJ)/%.c.o: %.c
	-@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	
# HexFiend requires more flags
$(OBJ)/HexFiend/%.m.o: HexFiend/%.m
	-@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) $(OCFLAGS) -c $< -o $@ -fno-objc-arc -include HexFiend/HexFiend_2_Framework_Prefix.pch
	
$(OBJ)/%.m.o: %.m
	-@$(MKDIR) -p $(dir $@)
	$(CC) $(CFLAGS) $(OCFLAGS) -c $< -o $@

# Cocoa Port

Shaders:$(shell ls Shaders/*.fsh)

$(BIN)/Sameboy.app: $(BIN)/Sameboy.app/Contents/MacOS/Sameboy \
					$(shell ls Cocoa/*.icns) \
					Cocoa/License.html \
					Cocoa/info.plist \
					$(BIN)/BootROMs/dmg_boot.bin \
					$(BIN)/BootROMs/cgb_boot.bin \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/Document.nib \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/MainMenu.nib \
					$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/Preferences.nib \
					Shaders
	$(MKDIR) -p $(BIN)/Sameboy.app/Contents/Resources
	cp Cocoa/*.icns $(BIN)/BootROMs/dmg_boot.bin $(BIN)/BootROMs/cgb_boot.bin $(BIN)/Sameboy.app/Contents/Resources/
	sed s/@VERSION/$(VERSION)/ < Cocoa/info.plist > $(BIN)/Sameboy.app/Contents/info.plist
	cp Cocoa/License.html $(BIN)/Sameboy.app/Contents/Resources/Credits.html
	$(MKDIR) -p $(BIN)/Sameboy.app/Contents/Resources/Shaders
	cp Shaders/*.fsh $(BIN)/Sameboy.app/Contents/Resources/Shaders

$(BIN)/Sameboy.app/Contents/MacOS/Sameboy: $(CORE_OBJECTS) $(COCOA_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) -framework OpenGL -framework AudioUnit
ifeq ($(CONF), release)
	strip $@
endif

$(BIN)/Sameboy.app/Contents/Resources/Base.lproj/%.nib: Cocoa/%.xib
	ibtool --compile $@ $^
	
# SDL Port

# Unix versions build only one binary
$(BIN)/sdl/sameboy: $(CORE_OBJECTS) $(SDL_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) $(SDL_LDFLAGS)
ifeq ($(CONF), release)
	strip $@
endif

# Windows version builds two, one with a conole and one without it
$(BIN)/sdl/sameboy.exe: $(CORE_OBJECTS) $(SDL_OBJECTS) $(OBJ)/Windows/resources.o
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) $(SDL_LDFLAGS) -Wl,/subsystem:windows

$(BIN)/sdl/sameboy_debugger.exe: $(CORE_OBJECTS) $(SDL_OBJECTS) $(OBJ)/Windows/resources.o
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) $(SDL_LDFLAGS) -Wl,/subsystem:console

$(OBJ)/%.res: %.rc
	-@$(MKDIR) -p $(dir $@)
	rc /fo $@ /dVERSION=\"$(VERSION)\" $^ 

%.o: %.res
	cvtres /OUT:"$@" $^

# We must provide SDL.dll with the Windows port. This is an AWFUL HACK to find it.
SPACE :=
SPACE +=
$(BIN)/sdl/SDL.dll:
	@$(eval POTENTIAL_MATCHES := $(subst @@@," ",$(patsubst %,%/SDL.dll,$(subst ;,$(SPACE),$(subst $(SPACE),@@@,$(lib))))))
	@$(eval MATCH := $(shell ls $(POTENTIAL_MATCHES) 2> NUL | head -n 1))
	cp "$(MATCH)" $@

$(BIN)/sdl/%.bin: $(BIN)/BootROMs/%.bin
	-@$(MKDIR) -p $(dir $@)
	cp -f $^ $@
	
$(BIN)/sdl/LICENSE: LICENSE
	cp -f $^ $@

# Boot ROMs

$(BIN)/BootROMs/%.bin: BootROMs/%.asm
	-@$(MKDIR) -p $(dir $@)
	cd BootROMs && rgbasm -o ../$@.tmp ../$<
	rgblink -o $@.tmp2 $@.tmp
	head -c $(if $(findstring dmg,$@), 256, 2304) $@.tmp2 > $@
	@rm $@.tmp $@.tmp2

# Clean

clean:
	rm -rf build