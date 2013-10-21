ifeq ($(platform),)
  phoenixflags = $(cppflags) $(flags) -DPHOENIX_REFERENCE
  phoenixlink =
else ifeq ($(platform),windows)
  phoenixflags = $(cppflags) $(flags) -DPHOENIX_WINDOWS
  phoenixlink = -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32 -lshlwapi
else ifeq ($(platform),macosx)
  phoenixflags = $(objcppflags) $(flags) -DPHOENIX_COCOA
  phoenixlink = -framework Cocoa -framework Carbon
else
  ifeq ($(phoenix),)
    phoenix := gtk
  endif

  ifeq ($(phoenix),gtk)
    phoenixflags = $(cppflags) $(flags) -DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`
    phoenixlink = `pkg-config --libs gtk+-2.0`
  endif

  ifeq ($(phoenix),qt)
    phoenixflags = $(cppflags) $(flags) -DPHOENIX_QT `pkg-config --cflags QtCore QtGui`
    phoenixlink = `pkg-config --libs QtCore QtGui`
  endif
endif
