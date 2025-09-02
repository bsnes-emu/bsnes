ifeq ($(platform),windows)
  ifeq ($(hiro),)
    hiro := windows
  endif

  ifeq ($(hiro),windows)
    hiro.flags   = $(flags.cpp) -DHIRO_WINDOWS
    hiro.options = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi -ldwmapi
  endif

  ifeq ($(hiro),gtk3)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0) -Wno-deprecated-declarations
    hiro.options = $(shell pkg-config --libs gtk+-3.0)
  endif
endif

ifeq ($(platform),macos)
  ifeq ($(hiro),)
    hiro := cocoa
  endif

  ifeq ($(hiro),cocoa)
    hiro.flags   = $(flags.objcpp) -w -DHIRO_COCOA
    hiro.options = -framework Cocoa -framework Carbon -framework IOKit -framework Security
  endif
endif

ifneq ($(filter $(platform),linux bsd),)
  ifeq ($(hiro),)
    hiro := gtk3
  endif

  ifeq ($(hiro),gtk3)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0) -Wno-deprecated-declarations
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs gtk+-3.0)
  endif

  ifeq ($(hiro),gtk3-se)
    flags       += -DHiro_SourceEdit
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0 gtksourceview-3.0) -Wno-deprecated-declarations
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs gtk+-3.0 gtksourceview-3.0)
  endif

  ifeq ($(hiro),qt6)
    moc = $(shell pkg-config --variable=libexecdir Qt6Core)/moc
    hiro.flags   = $(flags.cpp) -DHIRO_QT=6 -fPIC $(shell pkg-config --cflags Qt6Core Qt6Gui Qt6Widgets)
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs Qt6Core Qt6Gui Qt6Widgets)
  endif

  ifeq ($(hiro),qt5)
    moc = $(shell pkg-config --variable=host_bins Qt5Core)/moc
    hiro.flags   = $(flags.cpp) -DHIRO_QT=5 -fPIC $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets)
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets)
  endif
endif

ifeq ($(hiro.resource),)
  hiro.resource := $(hiro.path)/windows/hiro.rc
endif

hiro.objects := \
  $(object.path)/hiro-$(hiro).o \
  $(if $(filter windows,$(hiro)),$(object.path)/hiro-resource.o)

$(object.path)/hiro-$(hiro).o: $(hiro.path)/hiro.cpp
	$(if $(filter qt%,$(hiro)),$(info Compiling $(hiro.path)/qt/qt.moc ...))
	$(if $(filter qt%,$(hiro)),@$(moc) -i -o $(hiro.path)/qt/qt.moc $(hiro.path)/qt/qt.hpp)
	$(info Compiling $(subst ../,,$<) ...)
	@$(compiler) $(hiro.flags) $(flags) $(flags.deps) -c $< -o $@

$(object.path)/hiro-resource.o: $(hiro.resource)
	$(info Compiling $(subst ../,,$<) ...)
	@$(windres) $< $@

hiro.verbose:
	$(info hiro Target:)
	$(info $([space]) $(hiro))
