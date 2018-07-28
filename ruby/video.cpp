#if defined(VIDEO_CGL)
  #include <ruby/video/cgl.cpp>
#endif

#if defined(VIDEO_DIRECT3D)
  #include <ruby/video/direct3d.cpp>
#endif

#if defined(VIDEO_DIRECTDRAW)
  #include <ruby/video/directdraw.cpp>
#endif

#if defined(VIDEO_GDI)
  #include <ruby/video/gdi.cpp>
#endif

#if defined(VIDEO_GLX)
  #include <ruby/video/glx.cpp>
#endif

#if defined(VIDEO_GLX2)
  #include <ruby/video/glx2.cpp>
#endif

#if defined(VIDEO_WGL)
  #include <ruby/video/wgl.cpp>
#endif

#if defined(VIDEO_XSHM)
  #include <ruby/video/xshm.cpp>
#endif

#if defined(VIDEO_XVIDEO)
  #include <ruby/video/xvideo.cpp>
#endif

namespace ruby {

auto Video::setExclusive(bool exclusive) -> bool {
  _exclusive = exclusive;
  return true;
}

auto Video::setContext(uintptr context) -> bool {
  _context = context;
  return true;
}

auto Video::setBlocking(bool blocking) -> bool {
  _blocking = blocking;
  return true;
}

auto Video::setFlush(bool flush) -> bool {
  _flush = flush;
  return true;
}

auto Video::setFormat(string format) -> bool {
  _format = format;
  return true;
}

auto Video::setSmooth(bool smooth) -> bool {
  _smooth = smooth;
  return true;
}

auto Video::setShader(string shader) -> bool {
  _shader = shader;
  return true;
}

auto Video::onUpdate(const function<void (uint, uint)>& onUpdate) -> void {
  _onUpdate = onUpdate;
}

auto Video::doUpdate(uint width, uint height) -> void {
  if(_onUpdate) return _onUpdate(width, height);
}

//protected functions

//static functions

auto Video::create(string driver) -> Video* {
  Video* video = nullptr;
  if(!driver) driver = optimalDriver();

  #if defined(VIDEO_CGL)
  if(driver == "OpenGL") video = new VideoCGL;
  #endif

  #if defined(VIDEO_DIRECT3D)
  if(driver == "Direct3D") video = new VideoDirect3D;
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  if(driver == "DirectDraw") video = new VideoDirectDraw;
  #endif

  #if defined(VIDEO_GDI)
  if(driver == "GDI") video = new VideoGDI;
  #endif

  #if defined(VIDEO_GLX)
  if(driver == "OpenGL") video = new VideoGLX;
  #endif

  #if defined(VIDEO_GLX2)
  if(driver == "OpenGL2") video = new VideoGLX2;
  #endif

  #if defined(VIDEO_WGL)
  if(driver == "OpenGL") video = new VideoWGL;
  #endif

  #if defined(VIDEO_XSHM)
  if(driver == "XShm") video = new VideoXShm;
  #endif

  #if defined(VIDEO_XVIDEO)
  if(driver == "XVideo") video = new VideoXVideo;
  #endif

  if(!video) video = new Video;
  return video;
}

auto Video::optimalDriver() -> string {
  #if defined(VIDEO_WGL)
  return "OpenGL";
  #elif defined(VIDEO_DIRECT3D)
  return "Direct3D";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL";
  #elif defined(VIDEO_GLX)
  return "OpenGL";
  #elif defined(VIDEO_GLX2)
  return "OpenGL2";
  #elif defined(VIDEO_XVIDEO)
  return "XVideo";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #else
  return "None";
  #endif
}

auto Video::safestDriver() -> string {
  #if defined(VIDEO_DIRECT3D)
  return "Direct3D";
  #elif defined(VIDEO_WGL)
  return "OpenGL";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #elif defined(VIDEO_XVIDEO)
  return "XVideo";
  #elif defined(VIDEO_GLX2)
  return "OpenGL2";
  #elif defined(VIDEO_GLX)
  return "OpenGL";
  #else
  return "None";
  #endif
}

auto Video::availableDrivers() -> vector<string> {
  return {

  #if defined(VIDEO_WGL)
  "OpenGL",
  #endif

  #if defined(VIDEO_DIRECT3D)
  "Direct3D",
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  "DirectDraw",
  #endif

  #if defined(VIDEO_GDI)
  "GDI",
  #endif

  #if defined(VIDEO_CGL)
  "OpenGL",
  #endif

  #if defined(VIDEO_GLX)
  "OpenGL",
  #endif

  #if defined(VIDEO_GLX2)
  "OpenGL2",
  #endif

  #if defined(VIDEO_XVIDEO)
  "XVideo",
  #endif

  #if defined(VIDEO_XSHM)
  "XShm",
  #endif

  "None"};
}

}
