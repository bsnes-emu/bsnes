#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>

extern "C" auto XvShmCreateImage(Display*, XvPortID, int, char*, int, int, XShmSegmentInfo*) -> XvImage*;

struct VideoXVideo : Video {
  VideoXVideo() { initialize(); }
  ~VideoXVideo() { terminate(); }

  auto ready() -> bool { return _ready; }

  auto context() -> uintptr { return _context; }
  auto blocking() -> bool { return _blocking; }

  auto setContext(uintptr context) -> bool {
    if(_context == context) return true;
    _context = context;
    return initialize();
  }

  auto setBlocking(bool blocking) -> bool {
    if(_blocking == blocking) return true;
    _blocking = blocking;

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

  auto clear() -> void {
    if(!ready()) return;
    memory::fill<uint32_t>(_buffer, _bufferWidth * _bufferHeight);
    //clear twice in case video is double buffered ...
    output();
    output();
  }

  auto lock(uint32_t*& data, uint& pitch, uint width, uint height) -> bool {
    if(!ready()) return false;
    if(width != _width || height != _height) resize(_width = width, _height = height);
    pitch = _bufferWidth * 4;
    return data = _buffer;
  }

  auto unlock() -> void {
    if(!ready()) return;
  }

  auto output() -> void {
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

    switch(_format) {
    case XvFormatRGB32: renderRGB32(_width, _height); break;
    case XvFormatRGB24: renderRGB24(_width, _height); break;
    case XvFormatRGB16: renderRGB16(_width, _height); break;
    case XvFormatRGB15: renderRGB15(_width, _height); break;
    case XvFormatUYVY:  renderUYVY (_width, _height); break;
    case XvFormatYUY2:  renderYUY2 (_width, _height); break;
    case XvFormatYV12:  renderYV12 (_width, _height); break;
    case XvFormatI420:  renderI420 (_width, _height); break;
    }

    XvShmPutImage(_display, _port, _window, _gc, _image,
      0, 0, _width, _height,
      0, 0, target.width, target.height,
      true);
  }

private:
  auto initialize() -> bool {
    terminate();
    if(!_context) return false;

    _display = XOpenDisplay(nullptr);

    if(!XShmQueryExtension(_display)) {
      print("VideoXv: XShm extension not found.\n");
      return false;
    }

    //find an appropriate Xv port
    _port = -1;
    XvAdaptorInfo* adaptorInfo = nullptr;
    uint adaptorCount = 0;
    XvQueryAdaptors(_display, DefaultRootWindow(_display), &adaptorCount, &adaptorInfo);
    for(uint n : range(adaptorCount)) {
      //find adaptor that supports both input (memory->drawable) and image (drawable->screen) masks
      if(adaptorInfo[n].num_formats < 1) continue;
      if(!(adaptorInfo[n].type & XvInputMask)) continue;
      if(!(adaptorInfo[n].type & XvImageMask)) continue;

      _port = adaptorInfo[n].base_id;
      _depth = adaptorInfo[n].formats->depth;
      _visualID = adaptorInfo[n].formats->visual_id;
      break;
    }
    XvFreeAdaptorInfo(adaptorInfo);
    if(_port < 0) {
      print("VideoXv: failed to find valid XvPort.\n");
      return false;
    }

    //create child window to attach to parent window.
    //this is so that even if parent window visual depth doesn't match Xv visual
    //(common with composited windows), Xv can still render to child window.
    XWindowAttributes window_attributes;
    XGetWindowAttributes(_display, (Window)_context, &window_attributes);

    XVisualInfo visualTemplate;
    visualTemplate.visualid = _visualID;
    visualTemplate.screen = DefaultScreen(_display);
    visualTemplate.depth = _depth;
    visualTemplate.visual = 0;
    int visualMatches = 0;
    XVisualInfo* visualInfo = XGetVisualInfo(_display, VisualIDMask | VisualScreenMask | VisualDepthMask, &visualTemplate, &visualMatches);
    if(visualMatches < 1 || !visualInfo->visual) {
      if(visualInfo) XFree(visualInfo);
      print("VideoXv: unable to find Xv-compatible visual.\n");
      return false;
    }

    _colormap = XCreateColormap(_display, (Window)_context, visualInfo->visual, AllocNone);
    XSetWindowAttributes attributes;
    attributes.colormap = _colormap;
    attributes.border_pixel = 0;
    attributes.event_mask = StructureNotifyMask;
    _window = XCreateWindow(_display, /* parent = */ (Window)_context,
      /* x = */ 0, /* y = */ 0, window_attributes.width, window_attributes.height,
      /* border_width = */ 0, _depth, InputOutput, visualInfo->visual,
      CWColormap | CWBorderPixel | CWEventMask, &attributes);
    XFree(visualInfo);
    XSetWindowBackground(_display, _window, /* color = */ 0);
    XMapWindow(_display, _window);

    _gc = XCreateGC(_display, _window, 0, 0);

    int attributeCount = 0;
    XvAttribute* attributeList = XvQueryPortAttributes(_display, _port, &attributeCount);
    for(auto n : range(attributeCount)) {
      if(string{attributeList[n].name} == "XV_AUTOPAINT_COLORKEY") {
        //set colorkey to auto paint, so that Xv video output is always visible
        Atom atom = XInternAtom(_display, "XV_AUTOPAINT_COLORKEY", true);
        if(atom != None) XvSetPortAttribute(_display, _port, atom, 1);
      }
    }
    XFree(attributeList);

    //find optimal rendering format
    _format = XvFormatUnknown;
    int formatCount = 0;
    XvImageFormatValues* format = XvListImageFormats(_display, _port, &formatCount);

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvRGB && format[n].bits_per_pixel == 32) {
        _format = XvFormatRGB32;
        _fourCC = format[n].id;
        break;
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvRGB && format[n].bits_per_pixel == 24) {
        _format = XvFormatRGB24;
        _fourCC = format[n].id;
        break;
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvRGB && format[n].bits_per_pixel <= 16 && format[n].red_mask == 0xf800) {
        _format = XvFormatRGB16;
        _fourCC = format[n].id;
        break;
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvRGB && format[n].bits_per_pixel <= 16 && format[n].red_mask == 0x7c00) {
        _format = XvFormatRGB15;
        _fourCC = format[n].id;
        break;
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvYUV && format[n].bits_per_pixel == 16 && format[n].format == XvPacked) {
        if(format[n].component_order[0] == 'U' && format[n].component_order[1] == 'Y'
        && format[n].component_order[2] == 'V' && format[n].component_order[3] == 'Y'
        ) {
          _format = XvFormatUYVY;
          _fourCC = format[n].id;
          break;
        }
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvYUV && format[n].bits_per_pixel == 16 && format[n].format == XvPacked) {
        if(format[n].component_order[0] == 'Y' && format[n].component_order[1] == 'U'
        && format[n].component_order[2] == 'Y' && format[n].component_order[3] == 'V'
        ) {
          _format = XvFormatYUY2;
          _fourCC = format[n].id;
          break;
        }
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvYUV && format[n].bits_per_pixel == 12 && format[n].format == XvPlanar) {
        if(format[n].component_order[0] == 'Y' && format[n].component_order[1] == 'V'
        && format[n].component_order[2] == 'U' && format[n].component_order[3] == '\x00'
        ) {
          _format = XvFormatYV12;
          _fourCC = format[n].id;
          break;
        }
      }
    }

    if(_format == XvFormatUnknown) for(auto n : range(formatCount)) {
      if(format[n].type == XvYUV && format[n].bits_per_pixel == 12 && format[n].format == XvPlanar) {
        if(format[n].component_order[0] == 'Y' && format[n].component_order[1] == 'U'
        && format[n].component_order[2] == 'V' && format[n].component_order[3] == '\x00'
        ) {
          _format = XvFormatI420;
          _fourCC = format[n].id;
          break;
        }
      }
    }

    free(format);
    if(_format == XvFormatUnknown) {
      print("VideoXv: unable to find a supported image format.\n");
      return false;
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

    _image = XvShmCreateImage(_display, _port, _fourCC, 0, _bufferWidth, _bufferHeight, &_shmInfo);

    _shmInfo.shmid = shmget(IPC_PRIVATE, _image->data_size, IPC_CREAT | 0777);
    _shmInfo.shmaddr = _image->data = (char*)shmat(_shmInfo.shmid, 0, 0);
    _shmInfo.readOnly = false;
    XShmAttach(_display, &_shmInfo);

    delete[] _buffer;
    _buffer = new uint32_t[_bufferWidth * _bufferHeight];
  }

  auto renderRGB32(uint width, uint height) -> void {
    uint32_t* input = (uint32_t*)_buffer;
    uint32_t* output = (uint32_t*)_image->data;

    for(uint y : range(height)) {
      memory::copy<uint32_t>(output, input, width);
      input += _bufferWidth;
      output += _bufferWidth;
    }
  }

  auto renderRGB24(uint width, uint height) -> void {
    uint32_t* input = (uint32_t*)_buffer;
    uint8_t* output = (uint8_t*)_image->data;

    for(uint y : range(height)) {
      for(uint x : range(width)) {
        uint32_t p = *input++;
        *output++ = p >>  0;
        *output++ = p >>  8;
        *output++ = p >> 16;
      }

      input += (_bufferWidth - width);
      output += (_bufferWidth - width) * 3;
    }
  }

  auto renderRGB16(uint width, uint height) -> void {
    uint32_t* input = (uint32_t*)_buffer;
    uint16_t* output = (uint16_t*)_image->data;

    for(uint y : range(height)) {
      for(uint x : range(width)) {
        uint32_t p = toRGB16(*input++);
        *output++ = p;
      }

      input += _bufferWidth - width;
      output += _bufferWidth - width;
    }
  }

  auto renderRGB15(uint width, uint height) -> void {
    uint32_t* input = (uint32_t*)_buffer;
    uint16_t* output = (uint16_t*)_image->data;

    for(uint y : range(height)) {
      for(uint x : range(width)) {
        uint32_t p = toRGB15(*input++);
        *output++ = p;
      }

      input += _bufferWidth - width;
      output += _bufferWidth - width;
    }
  }

  auto renderUYVY(uint width, uint height) -> void {
    const uint32_t* input = (const uint32_t*)_buffer;
    uint16_t* output = (uint16_t*)_image->data;

    for(uint y : range(height)) {
      for(uint x : range(width >> 1)) {
        uint32_t p0 = toRGB16(*input++);
        uint32_t p1 = toRGB16(*input++);

        *output++ = _ytable[p0] << 8 | ((_utable[p0] + _utable[p1]) >> 1) << 0;
        *output++ = _ytable[p1] << 8 | ((_vtable[p0] + _vtable[p1]) >> 1) << 0;
      }

      input += _bufferWidth - width;
      output += _bufferWidth - width;
    }
  }

  auto renderYUY2(uint width, uint height) -> void {
    const uint32_t* input = (const uint32_t*)_buffer;
    uint16_t* output = (uint16_t*)_image->data;

    for(uint y : range(height)) {
      for(uint x : range(width >> 1)) {
        uint32_t p0 = toRGB16(*input++);
        uint32_t p1 = toRGB16(*input++);

        *output++ = ((_utable[p0] + _utable[p1]) >> 1) << 8 | _ytable[p0] << 0;
        *output++ = ((_vtable[p0] + _vtable[p1]) >> 1) << 8 | _ytable[p1] << 0;
      }

      input += _bufferWidth - width;
      output += _bufferWidth - width;
    }
  }

  auto renderYV12(uint width, uint height) -> void {
    const uint w = _bufferWidth, h = _bufferHeight;

    for(uint y : range(height >> 1)) {
      const uint32_t* input0 = (const uint32_t*)_buffer + (2 * y * w);
      const uint32_t* input1 = input0 + w;
      uint16_t* youtput0 = (uint16_t*)_image->data + ((2 * y * w) >> 1);
      uint16_t* youtput1 = youtput0 + (w >> 1);
      uint8_t* voutput = (uint8_t*)_image->data + (w * h) + ((2 * y * w) >> 2);
      uint8_t* uoutput = (uint8_t*)_image->data + (w * h) + ((w * h) >> 2) + ((2 * y * w) >> 2);

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
    const uint w = _bufferWidth, h = _bufferHeight;

    for(uint y : range(height >> 1)) {
      const uint32_t* input0 = (const uint32_t*)_buffer + (2 * y * w);
      const uint32_t* input1 = input0 + w;
      uint16_t* youtput0 = (uint16_t*)_image->data + ((2 * y * w) >> 1);
      uint16_t* youtput1 = youtput0 + (w >> 1);
      uint8_t* uoutput = (uint8_t*)_image->data + (w * h) + ((2 * y * w) >> 2);
      uint8_t* voutput = (uint8_t*)_image->data + (w * h) + ((w * h) >> 2) + ((2 * y * w) >> 2);

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
  uintptr _context = 0;
  bool _blocking = false;

  uint _width = 0;
  uint _height = 0;

  uint32_t* _buffer = nullptr;
  uint _bufferWidth = 0;
  uint _bufferHeight = 0;

  uint8_t* _ytable = nullptr;
  uint8_t* _utable = nullptr;
  uint8_t* _vtable = nullptr;

  enum XvFormat : uint {
    XvFormatRGB32,
    XvFormatRGB24,
    XvFormatRGB16,
    XvFormatRGB15,
    XvFormatUYVY,
    XvFormatYUY2,
    XvFormatYV12,
    XvFormatI420,
    XvFormatUnknown,
  };

  Display* _display = nullptr;
  GC _gc = 0;
  Window _window = 0;
  Colormap _colormap = 0;
  XShmSegmentInfo _shmInfo;

  int _port = -1;
  int _depth = 0;
  int _visualID = 0;

  XvImage* _image = nullptr;
  XvFormat _format = XvFormatUnknown;
  uint32_t _fourCC = 0;
};
