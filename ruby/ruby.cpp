#include <ruby/ruby.hpp>
using namespace nall;
using namespace ruby;

#undef deprecated
#undef mkdir
#undef noinline
#undef usleep

#if defined(DISPLAY_XORG)
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #include <X11/Xatom.h>
  #include <X11/extensions/Xrandr.h>
#elif defined(DISPLAY_QUARTZ)
  #include <nall/macos/guard.hpp>
  #include <Cocoa/Cocoa.h>
  #include <Carbon/Carbon.h>
  #include <nall/macos/guard.hpp>
#elif defined(DISPLAY_WINDOWS)
  #define far
  #include <mmsystem.h>
#endif

#include <ruby/video/video.cpp>
#include <ruby/audio/audio.cpp>
#include <ruby/input/input.cpp>
