# Make hacks
.INTERMEDIATE:

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

VERSION := 0.8
CONF ?= debug

BIN := build/bin
OBJ := build/obj
BOOTROMS_DIR ?= $(BIN)/BootROMs

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
SYSROOT := $(shell xcodebuild -sdk macosx -version Path 2> /dev/null)
CFLAGS += -F/Library/Frameworks
OCFLAGS += -x objective-c -fobjc-arc -Wno-deprecated-declarations -isysroot $(SYSROOT) -mmacosx-version-min=10.9
LDFLAGS += -framework AppKit -framework PreferencePanes -framework Carbon -framework QuartzCore
SDL_LDFLAGS := -F/Library/Frameworks -framework SDL
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
TESTER_TARGET := $(BIN)/tester/sameboy_tester.exe
else
SDL_TARGET := $(BIN)/sdl/sameboy
TESTER_TARGET := $(BIN)/tester/sameboy_tester
endif

cocoa: $(BIN)/SameBoy.app
quicklook: $(BIN)/SameBoy.qlgenerator
sdl: $(SDL_TARGET) $(BIN)/sdl/dmg_boot.bin $(BIN)/sdl/cgb_boot.bin $(BIN)/sdl/LICENSE $(BIN)/sdl/registers.sym
bootroms: $(BIN)/BootROMs/cgb_boot.bin $(BIN)/BootROMs/dmg_boot.bin
tester: $(TESTER_TARGET) $(BIN)/tester/dmg_boot.bin $(BIN)/tester/cgb_boot.bin
all: cocoa sdl tester

# Get a list of our source files and their respective object file targets

CORE_SOURCES := $(shell ls Core/*.c)
SDL_SOURCES := $(shell ls SDL/*.c)
TESTER_SOURCES := $(shell ls Tester/*.c)

ifeq ($(PLATFORM),Darwin)
COCOA_SOURCES := $(shell ls Cocoa/*.m) $(shell ls HexFiend/*.m)
QUICKLOOK_SOURCES := $(shell ls QuickLook/*.m) $(shell ls QuickLook/*.c)
SDL_SOURCES += $(shell ls SDL/*.m)
endif

CORE_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(CORE_SOURCES))
COCOA_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(COCOA_SOURCES))
QUICKLOOK_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(QUICKLOOK_SOURCES))
SDL_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(SDL_SOURCES))
TESTER_OBJECTS := $(patsubst %,$(OBJ)/%.o,$(TESTER_SOURCES))

# Automatic dependency generation

ifneq ($(MAKECMDGOALS),clean)
-include $(CORE_OBJECTS:.o=.dep)
ifneq ($(filter $(MAKECMDGOALS),sdl),)
-include $(SDL_OBJECTS:.o=.dep)
endif
ifneq ($(filter $(MAKECMDGOALS),tester),)
-include $(TESTER_OBJECTS:.o=.dep)
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

$(BIN)/SameBoy.app: $(BIN)/SameBoy.app/Contents/MacOS/SameBoy \
                    $(shell ls Cocoa/*.icns) \
                    Cocoa/License.html \
                    Cocoa/Info.plist \
                    Misc/registers.sym \
                    $(BIN)/SameBoy.app/Contents/Resources/dmg_boot.bin \
                    $(BIN)/SameBoy.app/Contents/Resources/cgb_boot.bin \
                    $(BIN)/SameBoy.app/Contents/Resources/Base.lproj/Document.nib \
                    $(BIN)/SameBoy.app/Contents/Resources/Base.lproj/MainMenu.nib \
                    $(BIN)/SameBoy.app/Contents/Resources/Base.lproj/Preferences.nib \
                    $(BIN)/SameBoy.qlgenerator \
                    Shaders
	$(MKDIR) -p $(BIN)/SameBoy.app/Contents/Resources
	cp Cocoa/*.icns Misc/registers.sym $(BIN)/SameBoy.app/Contents/Resources/
	sed s/@VERSION/$(VERSION)/ < Cocoa/Info.plist > $(BIN)/SameBoy.app/Contents/Info.plist
	cp Cocoa/License.html $(BIN)/SameBoy.app/Contents/Resources/Credits.html
	$(MKDIR) -p $(BIN)/SameBoy.app/Contents/Resources/Shaders
	cp Shaders/*.fsh $(BIN)/SameBoy.app/Contents/Resources/Shaders
	$(MKDIR) -p $(BIN)/SameBoy.app/Contents/Library/QuickLook/
	cp -rf $(BIN)/SameBoy.qlgenerator $(BIN)/SameBoy.app/Contents/Library/QuickLook/

$(BIN)/SameBoy.app/Contents/MacOS/SameBoy: $(CORE_OBJECTS) $(COCOA_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) -framework OpenGL -framework AudioUnit -framework AVFoundation -framework CoreVideo -framework CoreMedia
ifeq ($(CONF), release)
	strip $@
endif

$(BIN)/SameBoy.app/Contents/Resources/Base.lproj/%.nib: Cocoa/%.xib
	ibtool --compile $@ $^
	
# Quick Look generator

$(BIN)/SameBoy.qlgenerator: $(BIN)/SameBoy.qlgenerator/Contents/MacOS/SameBoyQL \
                            $(shell ls QuickLook/*.png) \
                            QuickLook/Info.plist \
                            $(BIN)/SameBoy.qlgenerator/Contents/Resources/cgb_boot_fast.bin
	$(MKDIR) -p $(BIN)/SameBoy.qlgenerator/Contents/Resources
	cp QuickLook/*.png $(BIN)/SameBoy.qlgenerator/Contents/Resources/
	sed s/@VERSION/$(VERSION)/ < QuickLook/Info.plist > $(BIN)/SameBoy.qlgenerator/Contents/Info.plist

# Currently, SameBoy.app includes two "copies" of each Core .o file once in the app itself and
# once in the QL Generator. It should probably become a dylib instead.
$(BIN)/SameBoy.qlgenerator/Contents/MacOS/SameBoyQL: $(CORE_OBJECTS) $(QUICKLOOK_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) -bundle -framework Cocoa -framework Quicklook
ifeq ($(CONF), release)
	strip -u -r -s QuickLook/exports.sym $@
endif

# cgb_boot_fast.bin is not a standard boot ROM, we don't expect it to exist in the user-provided
# boot ROM directory.
$(BIN)/SameBoy.qlgenerator/Contents/Resources/cgb_boot_fast.bin: $(BIN)/BootROMs/cgb_boot_fast.bin
	-@$(MKDIR) -p $(dir $@)
	cp -f $^ $@
	
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

# Tester

$(BIN)/tester/sameboy_tester: $(CORE_OBJECTS) $(TESTER_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)
ifeq ($(CONF), release)
	strip $@
endif

$(BIN)/tester/sameboy_tester.exe: $(CORE_OBJECTS) $(SDL_OBJECTS)
	-@$(MKDIR) -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS) -Wl,/subsystem:console

$(BIN)/sdl/%.bin $(BIN)/tester/%.bin: $(BOOTROMS_DIR)/%.bin
	-@$(MKDIR) -p $(dir $@)
	cp -f $^ $@
	
$(BIN)/SameBoy.app/Contents/Resources/%.bin: $(BOOTROMS_DIR)/%.bin
	-@$(MKDIR) -p $(dir $@)
	cp -f $^ $@
	
$(BIN)/sdl/LICENSE: LICENSE
	cp -f $^ $@

$(BIN)/sdl/registers.sym: Misc/registers.sym
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