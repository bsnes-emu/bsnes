ifeq ($(platform),)
  hiroflags = $(cppflags) $(flags) -DHIRO_REFERENCE
  hirolink =
endif

ifeq ($(platform),windows)
  ifeq ($(hiro),)
    hiro := windows
  endif

  ifeq ($(hiro),windows)
    hiroflags = $(cppflags) $(flags) -DHIRO_WINDOWS
    hirolink = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi
  endif

  ifeq ($(hiro),gtk2)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK=2 $(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
    hirolink = $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
  endif

  ifeq ($(hiro),gtk3)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0 gtksourceview-3.0)
    hirolink = $(shell pkg-config --libs gtk+-3.0 gtksourceview-3.0)
  endif
endif

ifeq ($(platform),macos)
  ifeq ($(hiro),)
    hiro := cocoa
  endif

  ifeq ($(hiro),cocoa)
    hiroflags = $(objcppflags) $(flags) -w -DHIRO_COCOA
    hirolink = -framework Cocoa -framework Carbon -framework Security
  endif
endif

ifneq ($(filter $(platform),linux bsd),)
  ifeq ($(hiro),)
    hiro := gtk2
  endif

  ifeq ($(hiro),gtk2)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK=2 $(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
    hirolink = -lX11 $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
  endif

  ifeq ($(hiro),gtk3)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK=3 $(shell pkg-config --cflags gtk+-3.0 gtksourceview-3.0)
    hirolink = -lX11 $(shell pkg-config --libs gtk+-3.0 gtksourceview-3.0)
  endif

  ifeq ($(hiro),qt4)
    moc = moc-qt4
    hiroflags = $(cppflags) $(flags) -DHIRO_QT=4 $(shell pkg-config --cflags QtCore QtGui)
    hirolink = -lX11 $(shell pkg-config --libs QtCore QtGui)
  endif

  ifeq ($(hiro),qt5)
    moc = /usr/local/lib/qt5/bin/moc
    hiroflags = -fPIC $(cppflags) $(flags) -DHIRO_QT=5 $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets)
    hirolink = -lX11 $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets)
  endif
endif
