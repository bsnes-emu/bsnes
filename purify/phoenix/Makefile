ifeq ($(platform),x)
  ifeq ($(phoenix),)
    phoenix := gtk
  endif

  ifeq ($(phoenix),gtk)
    phoenixflags := -DPHOENIX_GTK `pkg-config --cflags gtk+-2.0`
    phoenixlink := `pkg-config --libs gtk+-2.0`
  endif

  ifeq ($(phoenix),qt)
    phoenixflags := -DPHOENIX_QT `pkg-config --cflags QtCore QtGui`
    phoenixlink := `pkg-config --libs QtCore QtGui`
  endif
else ifeq ($(platform),win)
  phoenixflags := -DPHOENIX_WINDOWS
  phoenixlink := -lkernel32 -luser32 -lgdi32 -ladvapi32 -lole32 -lcomctl32 -lcomdlg32
else
  phoenixflags := -DPHOENIX_REFERENCE
  phoenixlink :=
endif
