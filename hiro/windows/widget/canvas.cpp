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
  if(auto& icon = state().icon) return {(int)icon.width(), (int)icon.height()};
  return {0, 0};
}

auto pCanvas::setColor(Color color) -> void {
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  update();
}

auto pCanvas::setGradient(Gradient gradient) -> void {
  update();
}

auto pCanvas::setIcon(const image& icon) -> void {
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
  bmi.bmiHeader.biSizeImage = pixels.size() * sizeof(uint32);
  void* bits = nullptr;
  HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
  if(bits) {
    auto source = (const uint8*)pixels.data();
    auto target = (uint8*)bits;
    for(auto n : range(width * height)) {
      target[0] = (source[0] * source[3]) / 255;
      target[1] = (source[1] * source[3]) / 255;
      target[2] = (source[2] * source[3]) / 255;
      target[3] = (source[3]);
      source += 4, target += 4;
    }
  }
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
  if(auto& icon = state().icon) {
    width = icon.width();
    height = icon.height();
  } else {
    width = self().geometry().width();
    height = self().geometry().height();
  }
  if(width <= 0 || height <= 0) return;

  pixels.reallocate(width * height);

  if(auto& icon = state().icon) {
    memory::copy(pixels.data(), icon.data(), width * height * sizeof(uint32));
  } else if(auto& gradient = state().gradient) {
    auto& colors = gradient.state.colors;
    image fill;
    fill.allocate(width, height);
    fill.gradient(colors[0].value(), colors[1].value(), colors[2].value(), colors[3].value());
    memory::copy(pixels.data(), fill.data(), fill.size());
  } else {
    uint32 color = state().color.value();
    for(auto& pixel : pixels) pixel = color;
  }
}

auto pCanvas::_redraw() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
