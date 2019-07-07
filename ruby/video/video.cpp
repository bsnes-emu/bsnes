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
  if(instance->exclusive == exclusive) return true;
  if(!instance->hasExclusive()) return false;
  if(!instance->setExclusive(instance->exclusive = exclusive)) return false;
  return true;
}

auto Video::setContext(uintptr context) -> bool {
  if(instance->context == context) return true;
  if(!instance->hasContext()) return false;
  if(!instance->setContext(instance->context = context)) return false;
  return true;
}

auto Video::setBlocking(bool blocking) -> bool {
  if(instance->blocking == blocking) return true;
  if(!instance->hasBlocking()) return false;
  if(!instance->setBlocking(instance->blocking = blocking)) return false;
  return true;
}

auto Video::setFlush(bool flush) -> bool {
  if(instance->flush == flush) return true;
  if(!instance->hasFlush()) return false;
  if(!instance->setFlush(instance->flush = flush)) return false;
  return true;
}

auto Video::setFormat(string format) -> bool {
  if(instance->format == format) return true;
  if(!instance->hasFormat(format)) return false;
  if(!instance->setFormat(instance->format = format)) return false;
  return true;
}

auto Video::setShader(string shader) -> bool {
  if(instance->shader == shader) return true;
  if(!instance->hasShader()) return false;
  if(!instance->setShader(instance->shader = shader)) return false;
  return true;
}

//

auto Video::clear() -> void {
  return instance->clear();
}

auto Video::size() -> Size {
  Size result;
  instance->size(result.width, result.height);
  return result;
}

auto Video::acquire(uint width, uint height) -> Acquire {
  Acquire result;
  if(instance->acquire(result.data, result.pitch, width, height)) return result;
  return {};
}

auto Video::release() -> void {
  return instance->release();
}

auto Video::output(uint width, uint height) -> void {
  return instance->output(width, height);
}

auto Video::poll() -> void {
  return instance->poll();
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
  self.instance.reset();
  if(!driver) driver = optimalDriver();

  #if defined(VIDEO_CGL)
  if(driver == "OpenGL 3.2") self.instance = new VideoCGL(*this);
  #endif

  #if defined(VIDEO_DIRECT3D)
  if(driver == "Direct3D 9.0") self.instance = new VideoDirect3D(*this);
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  if(driver == "DirectDraw 7.0") self.instance = new VideoDirectDraw(*this);
  #endif

  #if defined(VIDEO_GDI)
  if(driver == "GDI") self.instance = new VideoGDI(*this);
  #endif

  #if defined(VIDEO_GLX)
  if(driver == "OpenGL 3.2") self.instance = new VideoGLX(*this);
  #endif

  #if defined(VIDEO_GLX2)
  if(driver == "OpenGL 2.0") self.instance = new VideoGLX2(*this);
  #endif

  #if defined(VIDEO_WGL)
  if(driver == "OpenGL 3.2") self.instance = new VideoWGL(*this);
  #endif

  #if defined(VIDEO_XSHM)
  if(driver == "XShm") self.instance = new VideoXShm(*this);
  #endif

  #if defined(VIDEO_XVIDEO)
  if(driver == "XVideo") self.instance = new VideoXVideo(*this);
  #endif

  if(!self.instance) self.instance = new VideoDriver(*this);

  return self.instance->create();
}

auto Video::hasDrivers() -> vector<string> {
  return {

  #if defined(VIDEO_WGL)
  "OpenGL 3.2",
  #endif

  #if defined(VIDEO_DIRECT3D)
  "Direct3D 9.0",
  #endif

  #if defined(VIDEO_DIRECTDRAW)
  "DirectDraw 7.0",
  #endif

  #if defined(VIDEO_GDI)
  "GDI",
  #endif

  #if defined(VIDEO_CGL)
  "OpenGL 3.2",
  #endif

  #if defined(VIDEO_GLX)
  "OpenGL 3.2",
  #endif

  #if defined(VIDEO_GLX2)
  "OpenGL 2.0",
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
  return "OpenGL 3.2";
  #elif defined(VIDEO_DIRECT3D)
  return "Direct3D 9.0";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw 7.0";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL 3.2";
  #elif defined(VIDEO_GLX)
  return "OpenGL 3.2";
  #elif defined(VIDEO_GLX2)
  return "OpenGL 2.0";
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
  return "Direct3D 9.0";
  #elif defined(VIDEO_WGL)
  return "OpenGL 3.2";
  #elif defined(VIDEO_DIRECTDRAW)
  return "DirectDraw 7.0";
  #elif defined(VIDEO_GDI)
  return "GDI";
  #elif defined(VIDEO_CGL)
  return "OpenGL 3.2";
  #elif defined(VIDEO_XSHM)
  return "XShm";
  #elif defined(VIDEO_XVIDEO)
  return "XVideo";
  #elif defined(VIDEO_GLX2)
  return "OpenGL 2.0";
  #elif defined(VIDEO_GLX)
  return "OpenGL 3.2";
  #else
  return "None";
  #endif
}

}
