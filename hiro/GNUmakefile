ifeq ($(platform),)
  hiroflags = $(cppflags) $(flags) -DHIRO_REFERENCE
  hirolink =
else ifeq ($(platform),windows)
  ifeq ($(hiro),)
    hiro := windows
  endif

  ifeq ($(hiro),windows)
    hiroflags = $(cppflags) $(flags) -DHIRO_WINDOWS
    hirolink = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi
  endif

  ifeq ($(hiro),gtk)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK $(shell pkg-config --cflags gtk+-2.0 gtksourceview-2.0)
    hirolink = $(shell pkg-config --libs gtk+-2.0 gtksourceview-2.0)
  endif
else ifeq ($(platform),macosx)
  ifeq ($(hiro),)
    hiro := cocoa
  endif

  ifeq ($(hiro),cocoa)
    hiroflags = $(objcppflags) $(flags) -DHIRO_COCOA
    hirolink = -framework Cocoa -framework Carbon
  endif
else
  ifeq ($(hiro),)
    hiro := gtk
  endif

  ifeq ($(hiro),gtk)
    hiroflags = $(cppflags) $(flags) -DHIRO_GTK `pkg-config --cflags gtk+-2.0 gtksourceview-2.0`
    hirolink = `pkg-config --libs gtk+-2.0 gtksourceview-2.0` -lX11
  endif

  ifeq ($(hiro),qt)
    hiroflags = $(cppflags) $(flags) -DHIRO_QT `pkg-config --cflags QtCore QtGui`
    hirolink = `pkg-config --libs QtCore QtGui` -lX11
  endif
endif
