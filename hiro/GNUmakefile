ifeq ($(platform),)
  hiroflags = $(cppflags) $(flags) -DHIRO_REFERENCE
  hirolink =
else ifeq ($(platform),windows)
  hiroflags = $(cppflags) $(flags) -DHIRO_WINDOWS
  hirolink = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -luxtheme -lmsimg32 -lshlwapi
else ifeq ($(platform),macosx)
  hiroflags = $(objcppflags) $(flags) -DHIRO_COCOA
  hirolink = -framework Cocoa -framework Carbon
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
