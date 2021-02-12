ifeq ($(platform),windows)
  ifeq ($(hiro),)
    hiro := windows
  endif

  ifeq ($(hiro),windows)
    hiro.flags   = $(flags.cpp) -DHIRO_WINDOWS
    hiro.options = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi -ldwmapi
  endif

  ifeq ($(hiro),gtk2)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=2 $(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
    hiro.options = $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
  endif

  ifeq ($(hiro),gtk3)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0 gtksourceview-3.0) -Wno-deprecated-declarations
    hiro.options = $(shell pkg-config --libs gtk+-3.0 gtksourceview-3.0)
  endif
endif

ifeq ($(platform),macos)
  ifeq ($(hiro),)
    hiro := cocoa
  endif

  ifeq ($(hiro),cocoa)
    hiro.flags   = $(flags.objcpp) -w -DHIRO_COCOA
    hiro.options = -framework Cocoa -framework Carbon -framework Security
  endif
endif

ifneq ($(filter $(platform),linux bsd),)
  ifeq ($(hiro),)
    hiro := gtk2
  endif

  ifeq ($(hiro),gtk2)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=2 $(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
    ifeq ($(platform),bsd)
      hiro.options += -lc++
    endif
  endif

  ifeq ($(hiro),gtk3)
    hiro.flags   = $(flags.cpp) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0 gtksourceview-3.0) -Wno-deprecated-declarations
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs gtk+-3.0 gtksourceview-3.0)
  endif

  ifeq ($(hiro),qt4)
    moc = /usr/local/lib/qt4/bin/moc
    hiro.flags   = $(flags.cpp) -DHIRO_QT=4 $(shell pkg-config --cflags QtCore QtGui)
    hiro.options = -L/usr/local/lib -lX11 $(shell pkg-config --libs QtCore QtGui)
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
	$(info Compiling $< ...)
	@$(compiler) $(hiro.flags) $(flags) $(flags.deps) $(hiro.options) -c $< -o $@

$(object.path)/hiro-resource.o: $(hiro.resource)
	$(info Compiling $< ...)
	@$(windres) $< $@

hiro.verbose:
	$(info hiro Target:)
	$(info $([space]) $(hiro))
