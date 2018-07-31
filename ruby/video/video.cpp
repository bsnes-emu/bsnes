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
  if(driver->exclusive == exclusive) return true;
  if(!driver->hasExclusive()) return false;
  if(!driver->setExclusive(driver->exclusive = exclusive)) return false;
  return true;
}

auto Video::setContext(uintptr context) -> bool {
  if(driver->context == context) return true;
  if(!driver->hasContext()) return false;
  if(!driver->setContext(driver->context = context)) return false;
  return true;
}

auto Video::setBlocking(bool blocking) -> bool {
  if(driver->blocking == blocking) return true;
  if(!driver->hasBlocking()) return false;
  if(!driver->setBlocking(driver->blocking = blocking)) return false;
  return true;
}

auto Video::setFlush(bool flush) -> bool {
  if(driver->flush == flush) return true;
  if(!driver->hasFlush()) return false;
  if(!driver->setFlush(driver->flush = flush)) return false;
  return true;
}

auto Video::setFormat(string format) -> bool {
  if(driver->format == format) return true;
  if(!driver->hasFormat(format)) return false;
  if(!driver->setFormat(driver->format = format)) return false;
  return true;
}

auto Video::setSmooth(bool smooth) -> bool {
  if(driver->smooth == smooth) return true;
  if(!driver->hasSmooth()) return false;
  if(!driver->setSmooth(driver->smooth = smooth)) return false;
  return true;
}

auto Video::setShader(string shader) -> bool {
  if(driver->shader == shader) return true;
  if(!driver->hasShader()) return false;
  if(!driver->setShader(driver->shader = shader)) return false;
  return true;
}

//

auto Video::clear() -> void {
  return driver->clear();
}

auto Video::acquire(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
  return driver->acquire(data, pitch, width, height);
}

auto Video::release() -> void {
  return driver->release();
}

auto Video::output() -> void {
  return driver->output();
}

auto Video::poll() -> void {
  return driver->poll();
}

//

auto Video::onUpdate(const function<void (uint, uint)>& onUpdate) -> void {
  update = onUpdate;
}

auto Video::doUpdate(uint width, uint height) -> void {
  if(update) return update(width, height);
}

//

auto Video::create(string driver) -> bool {
  reset();
  if(!driver) driver = optimalDriver();

  #if defined(VIDEO_CGL)
  if(driver == "OpenGL") self.driver = new VideoCGL(*this);
  #endif

  #if defined(VIDEO_DIRECT3D)
  if(driver == "Direct3D") self.driver = new VideoDirect3D(*this);
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  if(driver == "DirectDraw") self.driver = new VideoDirectDraw(*this);
  #endif

  #if defined(VIDEO_GDI)
  if(driver == "GDI") self.driver = new VideoGDI(*this);
  #endif

  #if defined(VIDEO_GLX)
  if(driver == "OpenGL") self.driver = new VideoGLX(*this);
  #endif

  #if defined(VIDEO_GLX2)
  if(driver == "OpenGL2") self.driver = new VideoGLX2(*this);
  #endif

  #if defined(VIDEO_WGL)
  if(driver == "OpenGL") self.driver = new VideoWGL(*this);
  #endif

  #if defined(VIDEO_XSHM)
  if(driver == "XShm") self.driver = new VideoXShm(*this);
  #endif

  #if defined(VIDEO_XVIDEO)
  if(driver == "XVideo") self.driver = new VideoXVideo(*this);
  #endif

  if(!self.driver) self.driver = new VideoDriver(*this);

  return self.driver->create();
}

auto Video::hasDrivers() -> vector<string> {
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

}
