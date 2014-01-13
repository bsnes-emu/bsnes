namespace phoenix {

static LRESULT CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  Object* object = (Object*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(object == nullptr) return DefWindowProc(hwnd, msg, wparam, lparam);
  if(!dynamic_cast<Canvas*>(object)) return DefWindowProc(hwnd, msg, wparam, lparam);
  Canvas& canvas = (Canvas&)*object;

  if(msg == WM_DROPFILES) {
    lstring paths = DropPaths(wparam);
    if(paths.empty() == false) {
      if(canvas.onDrop) canvas.onDrop(paths);
    }
    return FALSE;
  }

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_ERASEBKGND) {
    //background is erased during WM_PAINT to prevent flickering
    return TRUE;
  }

  if(msg == WM_PAINT) {
    canvas.p.paint();
    return TRUE;
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT tracker = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd};
    TrackMouseEvent(&tracker);
    if(canvas.onMouseMove) canvas.onMouseMove({(int16_t)LOWORD(lparam), (int16_t)HIWORD(lparam)});
  }

  if(msg == WM_MOUSELEAVE) {
    if(canvas.onMouseLeave) canvas.onMouseLeave();
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    if(canvas.onMousePress) switch(msg) {
    case WM_LBUTTONDOWN: canvas.onMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: canvas.onMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: canvas.onMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    if(canvas.onMouseRelease) switch(msg) {
    case WM_LBUTTONUP: canvas.onMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: canvas.onMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: canvas.onMouseRelease(Mouse::Button::Right); break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void pCanvas::setDroppable(bool droppable) {
  DragAcceptFiles(hwnd, droppable);
}

void pCanvas::setGeometry(Geometry geometry) {
  if(canvas.state.width == 0 || canvas.state.height == 0) rasterize();
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width < geometry.width) {
    geometry.x += (geometry.width - width) / 2;
    geometry.width = width;
  }

  if(height < geometry.height) {
    geometry.y += (geometry.height - height) / 2;
    geometry.height = height;
  }

  pWidget::setGeometry(geometry);
}

void pCanvas::setMode(Canvas::Mode mode) {
  rasterize(), redraw();
}

void pCanvas::setSize(Size size) {
  rasterize(), redraw();
}

void pCanvas::constructor() {
  hwnd = CreateWindow(L"phoenix_canvas", L"", WS_CHILD, 0, 0, 0, 0, parentHwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&canvas);
  setDroppable(canvas.state.droppable);
  rasterize();
  synchronize();
}

void pCanvas::destructor() {
  release();
  DestroyWindow(hwnd);
}

void pCanvas::orphan() {
  destructor();
  constructor();
}

void pCanvas::paint() {
  if(surface == nullptr) return;

  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);

  uint32_t* data = surface;
  unsigned width = surfaceWidth;
  unsigned height = surfaceHeight;

  HDC hdc = CreateCompatibleDC(ps.hdc);
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;  //GDI stores bitmaps upside now; negative height flips bitmap
  bmi.bmiHeader.biSizeImage = width * height * sizeof(uint32_t);
  void* bits = nullptr;
  HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
  if(bits) memcpy(bits, data, width * height * sizeof(uint32_t));
  SelectObject(hdc, bitmap);

  BLENDFUNCTION bf;
  bf.BlendOp = AC_SRC_OVER;
  bf.BlendFlags = 0;
  bf.SourceConstantAlpha = 255;
  bf.AlphaFormat = AC_SRC_ALPHA;

  RECT rc;
  GetClientRect(hwnd, &rc);
  DrawThemeParentBackground(hwnd, ps.hdc, &rc);
  AlphaBlend(ps.hdc, 0, 0, width, height, hdc, 0, 0, width, height, bf);

  DeleteObject(bitmap);
  DeleteDC(hdc);

  EndPaint(hwnd, &ps);
}

void pCanvas::rasterize() {
  unsigned width = canvas.state.width;
  unsigned height = canvas.state.height;
  if(width == 0) width = widget.state.geometry.width;
  if(height == 0) height = widget.state.geometry.height;

  if(width != surfaceWidth || height != surfaceHeight) release();
  if(!surface) surface = new uint32_t[width * height];

  if(canvas.state.mode == Canvas::Mode::Color) {
    nall::image image;
    image.allocate(width, height);
    image.fill(canvas.state.color.argb());
    memcpy(surface, image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Gradient) {
    nall::image image;
    image.allocate(width, height);
    image.linearGradient(
      canvas.state.gradient[0].argb(), canvas.state.gradient[1].argb(), canvas.state.gradient[2].argb(), canvas.state.gradient[3].argb()
    );
    memcpy(surface, image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Image) {
    nall::image image = canvas.state.image;
    image.scale(width, height);
    image.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    memcpy(surface, image.data, image.size);
  }

  if(canvas.state.mode == Canvas::Mode::Data) {
    if(width == canvas.state.width && height == canvas.state.height) {
      memcpy(surface, canvas.state.data, width * height * sizeof(uint32_t));
    } else {
      memset(surface, 0x00, width * height * sizeof(uint32_t));
    }
  }

  surfaceWidth = width;
  surfaceHeight = height;
}

void pCanvas::redraw() {
  InvalidateRect(hwnd, 0, false);
}

void pCanvas::release() {
  if(surface) {
    delete[] surface;
    surface = nullptr;
    surfaceWidth = 0;
    surfaceHeight = 0;
  }
}

}
