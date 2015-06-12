#if defined(Hiro_Canvas)

namespace hiro {

static auto CALLBACK Canvas_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!object) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto canvas = dynamic_cast<mCanvas*>(object);
  if(!canvas) return DefWindowProc(hwnd, msg, wparam, lparam);

  if(msg == WM_DROPFILES) {
    if(auto paths = DropPaths(wparam)) canvas->doDrop(paths);
    return false;
  }

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_ERASEBKGND) {
    //background is erased during WM_PAINT to prevent flickering
    return true;
  }

  if(msg == WM_PAINT) {
    if(auto self = canvas->self()) self->_paint();
    return true;
  }

  if(msg == WM_MOUSEMOVE) {
    TRACKMOUSEEVENT tracker{sizeof(TRACKMOUSEEVENT), TME_LEAVE, hwnd};
    TrackMouseEvent(&tracker);
    canvas->doMouseMove({(int16_t)LOWORD(lparam), (int16_t)HIWORD(lparam)});
  }

  if(msg == WM_MOUSELEAVE) {
    canvas->doMouseLeave();
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    switch(msg) {
    case WM_LBUTTONDOWN: canvas->doMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: canvas->doMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: canvas->doMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    switch(msg) {
    case WM_LBUTTONUP: canvas->doMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: canvas->doMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: canvas->doMouseRelease(Mouse::Button::Right); break;
    }
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pCanvas::construct() -> void {
  hwnd = CreateWindow(L"hiroCanvas", L"", WS_CHILD, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  setDroppable(state().droppable);
  update();
}

auto pCanvas::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pCanvas::minimumSize() const -> Size {
  return {max(0, state().size.width()), max(0, state().size.height())};
}

auto pCanvas::setColor(Color color) -> void {
  mode = Mode::Color;
  update();
}

auto pCanvas::setData(Size size) -> void {
  mode = Mode::Data;
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  update();
}

auto pCanvas::setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> void {
  mode = Mode::Gradient;
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
  mode = Mode::Icon;
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  _redraw();
}

auto pCanvas::_paint() -> void {
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);

  HDC hdc = CreateCompatibleDC(ps.hdc);
  BITMAPINFO bmi;
  memset(&bmi, 0, sizeof(BITMAPINFO));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;  //GDI stores bitmaps upside now; negative height flips bitmap
  bmi.bmiHeader.biSizeImage = pixels.size() * sizeof(uint32_t);
  void* bits = nullptr;
  HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
  if(bits) memory::copy(bits, pixels.data(), pixels.size() * sizeof(uint32_t));
  SelectObject(hdc, bitmap);

  RECT rc;
  GetClientRect(hwnd, &rc);
  DrawThemeParentBackground(hwnd, ps.hdc, &rc);

  BLENDFUNCTION bf{AC_SRC_OVER, 0, (BYTE)255, AC_SRC_ALPHA};
  AlphaBlend(ps.hdc, 0, 0, width, height, hdc, 0, 0, width, height, bf);

  DeleteObject(bitmap);
  DeleteDC(hdc);

  EndPaint(hwnd, &ps);
}

auto pCanvas::_rasterize() -> void {
  if(mode == Mode::Color || mode == Mode::Gradient) {
    width = self().geometry().width();
    height = self().geometry().height();
  } else {
    width = state().size.width();
    height = state().size.height();
  }
  if(width <= 0 || height <= 0) return;

  pixels.reallocate(width * height);

  if(mode == Mode::Color) {
    uint32_t color = state().color.value();
    for(auto& pixel : pixels) pixel = color;
  }

  if(mode == Mode::Gradient) {
    image fill;
    fill.allocate(width, height);
    fill.gradient(
      state().gradient[0].value(), state().gradient[1].value(),
      state().gradient[2].value(), state().gradient[3].value()
    );
    memory::copy(pixels.data(), fill.data, fill.size);
  }

  if(mode == Mode::Icon) {
    auto icon = state().icon;
    icon.scale(width, height);
    icon.transform(0, 32, 255u << 24, 255u << 16, 255u << 8, 255u << 0);
    memory::copy(pixels.data(), icon.data, icon.size);
  }

  if(mode == Mode::Data) {
    memory::copy(
      pixels.data(), pixels.size() * sizeof(uint32_t),
      state().data.data(), state().data.size() * sizeof(uint32_t)
    );
  }
}

auto pCanvas::_redraw() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
