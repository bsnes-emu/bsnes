/* Global Headers */

#if defined(PLATFORM_XORG)
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #include <X11/Xatom.h>
#elif defined(PLATFORM_MACOSX)
  #define decimal CocoaDecimal
  #include <Cocoa/Cocoa.h>
  #include <Carbon/Carbon.h>
  #undef decimal
#elif defined(PLATFORM_WINDOWS)
  #define _WIN32_WINNT 0x0501
  #include <windows.h>
#endif

using namespace nall;

/* Video */

#define DeclareVideo(Name) \
  struct Video##Name : Video { \
    bool cap(const string& name) { return p.cap(name); } \
    any get(const string& name) { return p.get(name); } \
    bool set(const string& name, const any& value) { return p.set(name, value); } \
    \
    bool lock(uint32_t*& data, unsigned& pitch, unsigned width, unsigned height) { return p.lock(data, pitch, width, height); } \
    void unlock() { p.unlock(); } \
    \
    void clear() { p.clear(); } \
    void refresh() { p.refresh(); } \
    bool init() { return p.init(); } \
    void term() { p.term(); } \
    \
    Video##Name() : p(*new pVideo##Name) {} \
    ~Video##Name() { delete &p; } \
  \
  private: \
    pVideo##Name& p; \
  };

#ifdef VIDEO_CGL
  #include <ruby/video/cgl.cpp>
#endif

#ifdef VIDEO_DIRECT3D
  #include <ruby/video/direct3d.cpp>
#endif

#ifdef VIDEO_DIRECTDRAW
  #include <ruby/video/directdraw.cpp>
#endif

#ifdef VIDEO_GDI
  #include <ruby/video/gdi.cpp>
#endif

#ifdef VIDEO_GLX
  #include <ruby/video/glx.cpp>
#endif

#ifdef VIDEO_SDL
  #include <ruby/video/sdl.cpp>
#endif

#ifdef VIDEO_WGL
  #include <ruby/video/wgl.cpp>
#endif

#ifdef VIDEO_XSHM
  #include <ruby/video/xshm.cpp>
#endif

#ifdef VIDEO_XV
  #include <ruby/video/xv.cpp>
#endif

/* Audio */

#define DeclareAudio(Name) \
  struct Audio##Name : Audio { \
    bool cap(const string& name) { return p.cap(name); } \
    any get(const string& name) { return p.get(name); } \
    bool set(const string& name, const any& value) { return p.set(name, value); } \
    \
    void sample(uint16_t left, uint16_t right) { p.sample(left, right); } \
    void clear() { p.clear(); } \
    bool init() { return p.init(); } \
    void term() { p.term(); } \
    \
    Audio##Name() : p(*new pAudio##Name) {} \
    ~Audio##Name() { delete &p; } \
  \
  private: \
    pAudio##Name& p; \
  };

#ifdef AUDIO_ALSA
  #include <ruby/audio/alsa.cpp>
#endif

#ifdef AUDIO_AO
  #include <ruby/audio/ao.cpp>
#endif

#ifdef AUDIO_DIRECTSOUND
  #include <ruby/audio/directsound.cpp>
#endif

#ifdef AUDIO_OPENAL
  #include <ruby/audio/openal.cpp>
#endif

#ifdef AUDIO_OSS
  #include <ruby/audio/oss.cpp>
#endif

#ifdef AUDIO_PULSEAUDIO
  #include <ruby/audio/pulseaudio.cpp>
#endif

#ifdef AUDIO_PULSEAUDIOSIMPLE
  #include <ruby/audio/pulseaudiosimple.cpp>
#endif

#ifdef AUDIO_XAUDIO2
  #include <ruby/audio/xaudio2.cpp>
#endif

/* Input */

#define DeclareInput(Name) \
  struct Input##Name : Input { \
    bool cap(const string& name) { return p.cap(name); } \
    any get(const string& name) { return p.get(name); } \
    bool set(const string& name, const any& value) { return p.set(name, value); } \
    \
    bool acquire() { return p.acquire(); } \
    bool unacquire() { return p.unacquire(); } \
    bool acquired() { return p.acquired(); } \
    \
    vector<HID::Device*> poll() { return p.poll(); } \
    bool rumble(uint64_t id, bool enable) { return p.rumble(id, enable); } \
    bool init() { return p.init(); } \
    void term() { p.term(); } \
    \
    Input##Name() : p(*new pInput##Name) {} \
    ~Input##Name() { delete &p; } \
  \
  private: \
    pInput##Name& p; \
  };

#ifdef INPUT_CARBON
  #include <ruby/input/carbon.cpp>
#endif

#ifdef INPUT_SDL
  #include <ruby/input/sdl.cpp>
#endif

#ifdef INPUT_UDEV
  #include <ruby/input/udev.cpp>
#endif

#ifdef INPUT_WINDOWS
  #include <ruby/input/windows.cpp>
#endif

#ifdef INPUT_XLIB
  #include <ruby/input/xlib.cpp>
#endif
