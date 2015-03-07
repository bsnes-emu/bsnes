#if defined(PLATFORM_WINDOWS)
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
  #include <gdk/gdk.h>
  #include <gdk/gdkwin32.h>
  #include <gdk/gdkkeysyms.h>
  #include <gtk/gtk.h>
  #include <gtksourceview/gtksourceview.h>
  #include <gtksourceview/gtksourcelanguagemanager.h>
  #include <gtksourceview/gtksourcestyleschememanager.h>
  #include <cairo.h>
  #include <nall/windows/registry.hpp>
  #include <nall/windows/utf8.hpp>
#endif

#if defined(PLATFORM_XORG)
  #include <nall/xorg/guard.hpp>
  #include <gdk/gdk.h>
  #include <gdk/gdkx.h>
  #include <gdk/gdkkeysyms.h>
  #include <gtk/gtk.h>
  #include <gtksourceview/gtksourceview.h>
  #include <gtksourceview/gtksourcelanguagemanager.h>
  #include <gtksourceview/gtksourcestyleschememanager.h>
  #include <cairo.h>
  #include <X11/Xatom.h>
  #include <nall/xorg/guard.hpp>
#endif
