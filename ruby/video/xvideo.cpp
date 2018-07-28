#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>

extern "C" auto XvShmCreateImage(Display*, XvPortID, int, char*, int, int, XShmSegmentInfo*) -> XvImage*;

struct VideoXVideo : Video {
  VideoXVideo() { initialize(); }
  ~VideoXVideo() { terminate(); }

  auto driver() -> string override { return "XVideo"; }
  auto ready() -> bool override { return _ready; }

  auto availableFormats() -> vector<string> override {
    return _formatNames;
  }

  auto hasContext() -> bool override { return true; }
  auto hasBlocking() -> bool override { return true; }
  auto hasFormat() -> bool override { return true; }

  auto setContext(uintptr context) -> bool override {
    if(context == Video::context()) return true;
    if(!Video::setContext(context)) return false;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool override {
    if(blocking == Video::blocking()) return true;
    if(!Video::setBlocking(blocking)) return false;

    bool result = false;
    Display* display = XOpenDisplay(nullptr);
    Atom atom = XInternAtom(display, "XV_SYNC_TO_VBLANK", true);
    if(atom != None && _port >= 0) {
      XvSetPortAttribute(display, _port, atom, _blocking);
      result = true;
    }
    XCloseDisplay(display);
    return result;
  }

  auto setFormat(string format) -> bool override {
    if(format == Video::format()) return true;
    if(!Video::setFormat(format)) return false;
    return initialize();
  }

  auto clear() -> void override {
    if(!ready()) return;
    memory::fill<uint32_t>(_buffer, _bufferWidth * _bufferHeight);
    //clear twice in case video is double buffered ...
    output();
    output();
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool override {
    if(!ready()) return false;
    if(width != _width || height != _height) resize(_width = width, _height = height);
    pitch = _bufferWidth * 4;
    return data = _buffer;
  }

  auto unlock() -> void override {
    if(!ready()) return;
  }

  auto output() -> void override {
    if(!ready()) return;

    XWindowAttributes target;
    XGetWindowAttributes(_display, _window, &target);

    //we must ensure that the child window is the same size as the parent window.
    //unfortunately, we cannot hook the parent window resize event notification,
    //as we did not create the parent window, nor have any knowledge of the toolkit used.
    //therefore, query each window size and resize as needed.
    XWindowAttributes parent;
    XGetWindowAttributes(_display, (Window)_context, &parent);
    if(target.width != parent.width || target.height != parent.height) {
      XResizeWindow(_display, _window, parent.width, parent.height);
    }

    //update target width and height attributes
    XGetWindowAttributes(_display, _window, &target);

    auto& name = _formatName;
    if(name == "RGB32") renderRGB32(_width, _height);
    if(name == "RGB24") renderRGB24(_width, _height);
    if(name == "RGB16") renderRGB16(_width, _height);
    if(name == "RGB15") renderRGB15(_width, _height);
    if(name == "UYVY" ) renderUYVY (_width, _height);
    if(name == "YUY2" ) renderYUY2 (_width, _height);
    if(name == "YV12" ) renderYV12 (_width, _height);
    if(name == "I420" ) renderI420 (_width, _height);

    XvShmPutImage(_display, _port, _window, _gc, _image,
      0, 0, _width, _height,
      0, 0, target.width, target.height,
      true);
  }

  auto poll() -> void override {
    while(XPending(_display)) {
      XEvent event;
      XNextEvent(_display, &event);
      if(event.type == Expose) {
        XWindowAttributes attributes;
        XGetWindowAttributes(_display, _window, &attributes);
        doUpdate(attributes.width, attributes.height);
      }
    }
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    _display = XOpenDisplay(nullptr);

    if(!XShmQueryExtension(_display)) {
      print("XVideo: XShm extension not found.\n");
      return false;
    }

    //find an appropriate Xv port
    _port = -1;
    int depth = 0;
    int visualID = 0;
    XvAdaptorInfo* adaptorInfo = nullptr;
    uint adaptorCount = 0;
    XvQueryAdaptors(_display, DefaultRootWindow(_display), &adaptorCount, &adaptorInfo);
    for(uint n : range(adaptorCount)) {
      //find adaptor that supports both input (memory->drawable) and image (drawable->screen) masks
      if(adaptorInfo[n].num_formats < 1) continue;
      if(!(adaptorInfo[n].type & XvInputMask)) continue;
      if(!(adaptorInfo[n].type & XvImageMask)) continue;

      _port = adaptorInfo[n].base_id;
      depth = adaptorInfo[n].formats->depth;
      visualID = adaptorInfo[n].formats->visual_id;
      break;
    }
    XvFreeAdaptorInfo(adaptorInfo);
    if(_port < 0) {
      print("XVideo: failed to find valid XvPort.\n");
      return false;
    }

    //create child window to attach to parent window.
    //this is so that even if parent window visual depth doesn't match Xv visual
    //(common with composited windows), Xv can still render to child window.
    XWindowAttributes windowAttributes;
    XGetWindowAttributes(_display, (Window)_context, &windowAttributes);

    XVisualInfo visualTemplate;
    visualTemplate.visualid = visualID;
    visualTemplate.screen = DefaultScreen(_display);
    visualTemplate.depth = depth;
    visualTemplate.visual = 0;
    int visualMatches = 0;
    auto visualInfo = XGetVisualInfo(_display, VisualIDMask | VisualScreenMask | VisualDepthMask, &visualTemplate, &visualMatches);
    if(visualMatches < 1 || !visualInfo->visual) {
      if(visualInfo) XFree(visualInfo);
      print("XVideo: unable to find Xv-compatible visual.\n");
      return false;
    }

    _colormap = XCreateColormap(_display, (Window)_context, visualInfo->visual, AllocNone);
    XSetWindowAttributes attributes = {};
    attributes.colormap = _colormap;
    attributes.border_pixel = 0;
    _window = XCreateWindow(_display, /* parent = */ (Window)_context,
      /* x = */ 0, /* y = */ 0, windowAttributes.width, windowAttributes.height,
      /* border_width = */ 0, depth, InputOutput, visualInfo->visual,
      CWColormap | CWBorderPixel | CWEventMask, &attributes);
    XSelectInput(_display, _window, ExposureMask);
    XFree(visualInfo);
    XSetWindowBackground(_display, _window, /* color = */ 0);
    XMapWindow(_display, _window);

    _gc = XCreateGC(_display, _window, 0, 0);

    int attributeCount = 0;
    auto attributeList = XvQueryPortAttributes(_display, _port, &attributeCount);
    for(auto n : range(attributeCount)) {
      if(string{attributeList[n].name} == "XV_AUTOPAINT_COLORKEY") {
        //set colorkey to auto paint, so that Xv video output is always visible
        Atom atom = XInternAtom(_display, "XV_AUTOPAINT_COLORKEY", true);
        if(atom != None) XvSetPortAttribute(_display, _port, atom, 1);
      }
    }
    XFree(attributeList);

    queryAvailableFormats();
    if(!_formatNames) {
      print("XVideo: unable to find a supported image format.\n");
      return false;
    }
    if(auto match = _formatNames.find(Video::format())) {
      _formatID = _formatIDs[match()];
      _formatName = _formatNames[match()];
    } else {
      _formatID = _formatIDs[0];
      _formatName = _formatNames[0];
      Video::setFormat(_formatName);
    }

    _ready = true;
    initializeTables();
    resize(_width = 256, _height = 256);
    clear();
    return true;
  }

  auto terminate() -> void {
    _ready = false;

    if(_image) {
      XShmDetach(_display, &_shmInfo);
      shmdt(_shmInfo.shmaddr);
      shmctl(_shmInfo.shmid, IPC_RMID, nullptr);
      XFree(_image);
      _image = nullptr;
    }

    if(_gc) {
      XFreeGC(_display, _gc);
      _gc = 0;
    }

    if(_window) {
      XUnmapWindow(_display, _window);
      _window = 0;
    }

    if(_colormap) {
      XFreeColormap(_display, _colormap);
      _colormap = 0;
    }

    if(_display) {
      XCloseDisplay(_display);
      _display = nullptr;
    }

    delete[] _buffer, _buffer = nullptr, _bufferWidth = 0, _bufferHeight = 0;
    delete[] _ytable, _ytable = nullptr;
    delete[] _utable, _utable = nullptr;
    delete[] _vtable, _vtable = nullptr;
  }

  auto queryAvailableFormats() -> void {
    auto& ids = _formatIDs;
    auto& names = _formatNames;

    ids.reset();
    names.reset();

    int count = 0;
    auto array = XvListImageFormats(_display, _port, &count);

    for(uint sort : range(8)) {
      for(uint n : range(count)) {
        auto id = array[n].id;
        auto type = array[n].type;
        auto format = array[n].format;
        auto depth = array[n].bits_per_pixel;
        auto redMask = array[n].red_mask;
        auto order = array[n].component_order;
        string components;
        for(uint n : range(4)) if(char c = order[n]) components.append(c);

        if(type == XvRGB) {
          if(sort == 0 && depth == 32) ids.append(id), names.append("RGB32");
          if(sort == 1 && depth == 24) ids.append(id), names.append("RGB24");
          if(sort == 2 && depth <= 16 && redMask == 0xf800) ids.append(id), names.append("RGB16");
          if(sort == 3 && depth <= 16 && redMask == 0x7c00) ids.append(id), names.append("RGB15");
        }

        if(type == XvYUV && format == XvPacked) {
          if(sort == 4 && depth == 16 && components == "UYVY") ids.append(id), names.append("UYVY");
          if(sort == 5 && depth == 16 && components == "YUYV") ids.append(id), names.append("YUY2");
        }

        if(type == XvYUV && format == XvPlanar) {
          if(sort == 6 && depth == 12 && components == "YVU" ) ids.append(id), names.append("YV12");
          if(sort == 7 && depth == 12 && components == "YUV" ) ids.append(id), names.append("I420");
        }
      }
    }

    free(array);
  }

  auto resize(uint width, uint height) -> void {
    if(_bufferWidth >= width && _bufferHeight >= height) return;
    _bufferWidth = max(width, _bufferWidth);
    _bufferHeight = max(height, _bufferHeight);

    //must round to be evenly divisible by 4
    if(uint round = _bufferWidth & 3) _bufferWidth += 4 - round;
    if(uint round = _bufferHeight & 3) _bufferHeight += 4 - round;

    _bufferWidth = bit::round(_bufferWidth);
    _bufferHeight = bit::round(_bufferHeight);

    if(_image) {
      XShmDetach(_display, &_shmInfo);
      shmdt(_shmInfo.shmaddr);
      shmctl(_shmInfo.shmid, IPC_RMID, nullptr);
      XFree(_image);
    }

    _image = XvShmCreateImage(_display, _port, _formatID, 0, _bufferWidth, _bufferHeight, &_shmInfo);

    _shmInfo.shmid = shmget(IPC_PRIVATE, _image->data_size, IPC_CREAT | 0777);
    _shmInfo.shmaddr = _image->data = (char*)shmat(_shmInfo.shmid, 0, 0);
    _shmInfo.readOnly = false;
    XShmAttach(_display, &_shmInfo);

    delete[] _buffer;
    _buffer = new uint32_t[_bufferWidth * _bufferHeight];
  }

  auto renderRGB32(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint32_t*)_image->data + y * (_image->pitches[0] >> 2);

      for(uint x : range(width)) {
        uint32_t p = *input++;
        *output++ = p;
      }
    }
  }

  auto renderRGB24(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint8_t*)_image->data + y * _image->pitches[0];

      for(uint x : range(width)) {
        uint32_t p = *input++;
        *output++ = p >>  0;
        *output++ = p >>  8;
        *output++ = p >> 16;
      }
    }
  }

  auto renderRGB16(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint16_t*)_image->data + y * (_image->pitches[0] >> 1);

      for(uint x : range(width)) {
        uint32_t p = toRGB16(*input++);
        *output++ = p;
      }

      input += _bufferWidth - width;
      output += _bufferWidth - width;
    }
  }

  auto renderRGB15(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint16_t*)_image->data + y * (_image->pitches[0] >> 1);

      for(uint x : range(width)) {
        uint32_t p = toRGB15(*input++);
        *output++ = p;
      }
    }
  }

  auto renderUYVY(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint16_t*)_image->data + y * (_image->pitches[0] >> 1);

      for(uint x : range(width >> 1)) {
        uint32_t p0 = toRGB16(*input++);
        uint32_t p1 = toRGB16(*input++);

        *output++ = _ytable[p0] << 8 | ((_utable[p0] + _utable[p1]) >> 1) << 0;
        *output++ = _ytable[p1] << 8 | ((_vtable[p0] + _vtable[p1]) >> 1) << 0;
      }
    }
  }

  auto renderYUY2(uint width, uint height) -> void {
    for(uint y : range(height)) {
      auto input = (const uint32_t*)_buffer + y * width;
      auto output = (uint16_t*)_image->data + y * (_image->pitches[0] >> 1);

      for(uint x : range(width >> 1)) {
        uint32_t p0 = toRGB16(*input++);
        uint32_t p1 = toRGB16(*input++);

        *output++ = ((_utable[p0] + _utable[p1]) >> 1) << 8 | _ytable[p0] << 0;
        *output++ = ((_vtable[p0] + _vtable[p1]) >> 1) << 8 | _ytable[p1] << 0;
      }
    }
  }

  auto renderYV12(uint width, uint height) -> void {
    for(uint y : range(height >> 1)) {
      auto input0 = (const uint32_t*)_buffer + (2 * y + 0) * width;
      auto input1 = (const uint32_t*)_buffer + (2 * y + 1) * width;
      auto youtput0 = (uint16_t*)_image->data + (_image->offsets[0] >> 1) + (2 * y + 0) * (_image->pitches[0] >> 1);
      auto youtput1 = (uint16_t*)_image->data + (_image->offsets[0] >> 1) + (2 * y + 1) * (_image->pitches[0] >> 1);
      auto voutput = (uint8_t*)_image->data + _image->offsets[1] + y * _image->pitches[1];
      auto uoutput = (uint8_t*)_image->data + _image->offsets[2] + y * _image->pitches[2];

      for(uint x : range(width >> 1)) {
        uint16_t p0 = toRGB16(*input0++);
        uint16_t p1 = toRGB16(*input0++);
        uint16_t p2 = toRGB16(*input1++);
        uint16_t p3 = toRGB16(*input1++);

        *youtput0++ = _ytable[p0] << 0 | _ytable[p1] << 8;
        *youtput1++ = _ytable[p2] << 0 | _ytable[p3] << 8;
        *voutput++ = (_vtable[p0] + _vtable[p1] + _vtable[p2] + _vtable[p3]) >> 2;
        *uoutput++ = (_utable[p0] + _utable[p1] + _utable[p2] + _utable[p3]) >> 2;
      }
    }
  }

  auto renderI420(uint width, uint height) -> void {
    for(uint y : range(height >> 1)) {
      auto input0 = (const uint32_t*)_buffer + (2 * y + 0) * width;
      auto input1 = (const uint32_t*)_buffer + (2 * y + 1) * width;
      auto youtput0 = (uint16_t*)_image->data + (_image->offsets[0] >> 1) + (2 * y + 0) * (_image->pitches[0] >> 1);
      auto youtput1 = (uint16_t*)_image->data + (_image->offsets[0] >> 1) + (2 * y + 1) * (_image->pitches[0] >> 1);
      auto uoutput = (uint8_t*)_image->data + _image->offsets[1] + y * _image->pitches[1];
      auto voutput = (uint8_t*)_image->data + _image->offsets[2] + y * _image->pitches[2];

      for(uint x : range(width >> 1)) {
        uint16_t p0 = toRGB16(*input0++);
        uint16_t p1 = toRGB16(*input0++);
        uint16_t p2 = toRGB16(*input1++);
        uint16_t p3 = toRGB16(*input1++);

        *youtput0++ = _ytable[p0] << 0 | _ytable[p1] << 8;
        *youtput1++ = _ytable[p2] << 0 | _ytable[p3] << 8;
        *uoutput++ = (_utable[p0] + _utable[p1] + _utable[p2] + _utable[p3]) >> 2;
        *voutput++ = (_vtable[p0] + _vtable[p1] + _vtable[p2] + _vtable[p3]) >> 2;
      }
    }
  }

  inline auto toRGB15(uint32_t rgb32) const -> uint16_t {
    return ((rgb32 >> 9) & 0x7c00) + ((rgb32 >> 6) & 0x03e0) + ((rgb32 >> 3) & 0x001f);
  }

  inline auto toRGB16(uint32_t rgb32) const -> uint16_t {
    return ((rgb32 >> 8) & 0xf800) + ((rgb32 >> 5) & 0x07e0) + ((rgb32 >> 3) & 0x001f);
  }

  auto initializeTables() -> void {
    _ytable = new uint8_t[65536];
    _utable = new uint8_t[65536];
    _vtable = new uint8_t[65536];

    for(uint n : range(65536)) {
      //extract RGB565 color data from i
      uint8_t r = (n >> 11) & 31, g = (n >> 5) & 63, b = (n) & 31;
      r = (r << 3) | (r >> 2);  //R5->R8
      g = (g << 2) | (g >> 4);  //G6->G8
      b = (b << 3) | (b >> 2);  //B5->B8

      //ITU-R Recommendation BT.601
      //double lr = 0.299, lg = 0.587, lb = 0.114;
      int y = int( +(double(r) * 0.257) + (double(g) * 0.504) + (double(b) * 0.098) +  16.0 );
      int u = int( -(double(r) * 0.148) - (double(g) * 0.291) + (double(b) * 0.439) + 128.0 );
      int v = int( +(double(r) * 0.439) - (double(g) * 0.368) - (double(b) * 0.071) + 128.0 );

      //ITU-R Recommendation BT.709
      //double lr = 0.2126, lg = 0.7152, lb = 0.0722;
      //int y = int( double(r) * lr + double(g) * lg + double(b) * lb );
      //int u = int( (double(b) - y) / (2.0 - 2.0 * lb) + 128.0 );
      //int v = int( (double(r) - y) / (2.0 - 2.0 * lr) + 128.0 );

      _ytable[n] = y < 0 ? 0 : y > 255 ? 255 : y;
      _utable[n] = u < 0 ? 0 : u > 255 ? 255 : u;
      _vtable[n] = v < 0 ? 0 : v > 255 ? 255 : v;
    }
  }

  bool _ready = false;

  uint _width = 0;
  uint _height = 0;

  uint32_t* _buffer = nullptr;
  uint _bufferWidth = 0;
  uint _bufferHeight = 0;

  uint8_t* _ytable = nullptr;
  uint8_t* _utable = nullptr;
  uint8_t* _vtable = nullptr;

  Display* _display = nullptr;
  GC _gc = 0;
  Window _window = 0;
  Colormap _colormap = 0;
  XShmSegmentInfo _shmInfo;

  int _port = -1;
  XvImage* _image = nullptr;

  vector<int> _formatIDs;
  vector<string> _formatNames;

  int _formatID = 0;
  string _formatName;
};
