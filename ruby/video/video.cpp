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

auto Video::setFullScreen(bool fullScreen) -> bool {
  if(instance->fullScreen == fullScreen) return true;
  if(!instance->hasFullScreen()) return false;
  if(!instance->setFullScreen(instance->fullScreen = fullScreen)) return false;
  return true;
}

auto Video::setMonitor(string monitor) -> bool {
  if(instance->monitor == monitor) return true;
  if(!instance->hasMonitor()) return false;
  if(!instance->setMonitor(instance->monitor = monitor)) return false;
  return true;
}

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

#if defined(DISPLAY_WINDOWS)
static auto CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
  vector<Video::Monitor>& monitors = *(vector<Video::Monitor>*)dwData;
  MONITORINFOEX mi{};
  mi.cbSize = sizeof(MONITORINFOEX);
  GetMonitorInfo(hMonitor, &mi);
  Video::Monitor monitor;
  string deviceName = (const char*)utf8_t(mi.szDevice);
  if(deviceName.beginsWith(R"(\\.\DISPLAYV)")) return true;  //ignore pseudo-monitors
  DISPLAY_DEVICE dd{};
  dd.cb = sizeof(DISPLAY_DEVICE);
  EnumDisplayDevices(mi.szDevice, 0, &dd, 0);
  string displayName = (const char*)utf8_t(dd.DeviceString);
  monitor.name = {1 + monitors.size(), ": ", displayName};
  monitor.primary = mi.dwFlags & MONITORINFOF_PRIMARY;
  monitor.x = lprcMonitor->left;
  monitor.y = lprcMonitor->top;
  monitor.width = lprcMonitor->right - lprcMonitor->left;
  monitor.height = lprcMonitor->bottom - lprcMonitor->top;
  monitors.append(monitor);
  return true;
}

auto Video::hasMonitors() -> vector<Monitor> {
  vector<Monitor> monitors;
  EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, (LPARAM)&monitors);
  vector<Monitor> sorted;
  for(auto& monitor : monitors) { if(monitor.primary == 1) sorted.append(monitor); }
  for(auto& monitor : monitors) { if(monitor.primary == 0) sorted.append(monitor); }
  return sorted;
}
#endif

#if defined(DISPLAY_QUARTZ)
static auto MonitorKeyArrayCallback(const void* key, const void* value, void* context) -> void {
  CFArrayAppendValue((CFMutableArrayRef)context, key);
}

auto Video::hasMonitors() -> vector<Monitor> {
  vector<Monitor> monitors;
  @autoreleasepool {
    uint count = [[NSScreen screens] count];
    for(uint index : range(count)) {
      auto screen = [[NSScreen screens] objectAtIndex:index];
      auto rectangle = [screen frame];
      Monitor monitor;
      monitor.name = {1 + monitors.size(), ": Monitor"};  //fallback in case name lookup fails
      monitor.primary = monitors.size() == 0;   //on macOS, the primary monitor is always the first monitor.
      monitor.x = rectangle.origin.x;
      monitor.y = rectangle.origin.y;
      monitor.width = rectangle.size.width;
      monitor.height = rectangle.size.height;
      //getting the name of the monitor on macOS: "Think Different"
      auto screenDictionary = [screen deviceDescription];
      auto screenID = [screenDictionary objectForKey:@"NSScreenNumber"];
      auto displayID = [screenID unsignedIntValue];
      auto displayPort = CGDisplayIOServicePort(displayID);
      auto dictionary = IODisplayCreateInfoDictionary(displayPort, 0);
      if(auto names = CFDictionaryGetValue(dictionary, CFSTR(kDisplayProductName))) {
        auto languageKeys = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
        CFDictionaryApplyFunction((CFDictionaryRef)names, MonitorKeyArrayCallback, (void*)languageKeys);
        auto orderLanguageKeys = CFBundleCopyPreferredLocalizationsFromArray(languageKeys);
        CFRelease(languageKeys);
        if(orderLanguageKeys && CFArrayGetCount(orderLanguageKeys)) {
          auto languageKey = CFArrayGetValueAtIndex(orderLanguageKeys, 0);
          auto localName = CFDictionaryGetValue((CFDictionaryRef)names, languageKey);
          monitor.name = {1 + monitors.size(), ": ", [(__bridge NSString*)localName UTF8String]};
          CFRelease(localName);
        }
        CFRelease(orderLanguageKeys);
      }
      CFRelease(dictionary);
      monitors.append(monitor);
    }
  }
  return monitors;
}
#endif

#if defined(DISPLAY_XORG)
auto Video::hasMonitors() -> vector<Monitor> {
  vector<Monitor> monitors;

  auto display = XOpenDisplay(nullptr);
  auto screen = DefaultScreen(display);
  auto rootWindow = RootWindow(display, screen);
  auto resources = XRRGetScreenResourcesCurrent(display, rootWindow);
  auto primary = XRRGetOutputPrimary(display, rootWindow);
  for(uint index : range(resources->noutput)) {
    Monitor monitor;
    auto output = XRRGetOutputInfo(display, resources, resources->outputs[index]);
    if(output->connection != RR_Connected || output->crtc == None) {
      XRRFreeOutputInfo(output);
      continue;
    }
    auto crtc = XRRGetCrtcInfo(display, resources, output->crtc);
    monitor.name = {1 + monitors.size(), ": ", output->name};  //fallback name
    monitor.primary = false;
    for(uint n : range(crtc->noutput)) monitor.primary |= crtc->outputs[n] == primary;
    monitor.x = crtc->x;
    monitor.y = crtc->y;
    monitor.width = crtc->width;
    monitor.height = crtc->height;
    //Linux: "Think Low-Level"
    Atom actualType;
    int actualFormat;
    unsigned long size;
    unsigned long bytesAfter;
    unsigned char* data = nullptr;
    auto property = XRRGetOutputProperty(
      display, resources->outputs[index],
      XInternAtom(display, "EDID", 1), 0, 384,
      0, 0, 0, &actualType, &actualFormat, &size, &bytesAfter, &data
    );
    if(size >= 128) {
      string name{"             "};
      //there are four lights! er ... descriptors. one of them is the monitor name.
      if(data[0x39] == 0xfc) memory::copy(name.get(), &data[0x3b], 13);
      if(data[0x4b] == 0xfc) memory::copy(name.get(), &data[0x4d], 13);
      if(data[0x5d] == 0xfc) memory::copy(name.get(), &data[0x5f], 13);
      if(data[0x6f] == 0xfc) memory::copy(name.get(), &data[0x71], 13);
      if(name.strip()) {  //format: "name\n   " -> "name"
        monitor.name = {1 + monitors.size(), ": ", name, " [", output->name, "]"};
      }
    }
    XFree(data);
    monitors.append(monitor);
    XRRFreeCrtcInfo(crtc);
    XRRFreeOutputInfo(output);
  }
  XRRFreeScreenResources(resources);
  XCloseDisplay(display);

  vector<Monitor> sorted;
  for(auto& monitor : monitors) { if(monitor.primary == 1) sorted.append(monitor); }
  for(auto& monitor : monitors) { if(monitor.primary == 0) sorted.append(monitor); }
  return sorted;
}
#endif

auto Video::monitor(string name) -> Monitor {
  auto monitors = Video::hasMonitors();
  //try to find by name if possible
  for(auto& monitor : monitors) {
    if(monitor.name == name) return monitor;
  }
  //fall back to primary if not found
  for(auto& monitor : monitors) {
    if(monitor.primary) return monitor;
  }
  //Video::monitors() should never let this occur
  Monitor monitor;
  monitor.name = "Primary";
  monitor.primary = true;
  monitor.x = 0;
  monitor.y = 0;
  monitor.width = 640;
  monitor.height = 480;
  return monitor;
}

}
