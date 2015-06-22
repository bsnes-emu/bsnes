#if defined(DISPLAY_WINDOWS)
  #define UNICODE
  #define WINVER 0x0601
  #define _WIN32_WINNT WINVER
  #define _WIN32_IE WINVER
  #define __MSVCRT_VERSION__ WINVER
  #define NOMINMAX
  #define TBS_TRANSPARENTBKGND 0x1000

  #include <winsock2.h>
  #include <windows.h>
  #include <windowsx.h>
  #include <commctrl.h>
  #include <uxtheme.h>
  #include <io.h>
  #include <shlobj.h>
  #include <cairo.h>
  #include <gdk/gdk.h>
  #include <gdk/gdkwin32.h>
  #include <gdk/gdkkeysyms.h>
  #include <gtk/gtk.h>
  #if defined(Hiro_SourceEdit)
    #include <gtksourceview/gtksourceview.h>
    #include <gtksourceview/gtksourcelanguagemanager.h>
    #include <gtksourceview/gtksourcestyleschememanager.h>
  #endif
  #include <nall/windows/registry.hpp>
  #include <nall/windows/utf8.hpp>
#endif

#if defined(DISPLAY_XORG)
  #include <nall/xorg/guard.hpp>
  #include <X11/Xatom.h>
  #include <cairo.h>
  #include <gdk/gdk.h>
  #include <gdk/gdkx.h>
  #include <gdk/gdkkeysyms.h>
  #include <gtk/gtk.h>
  #if defined(Hiro_SourceEdit)
    #include <gtksourceview/gtksourceview.h>
    #include <gtksourceview/gtksourcelanguagemanager.h>
    #include <gtksourceview/gtksourcestyleschememanager.h>
  #endif
  #include <nall/xorg/guard.hpp>
#endif
