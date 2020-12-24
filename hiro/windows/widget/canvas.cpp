#if defined(Hiro_Canvas)

namespace hiro {

auto pCanvas::construct() -> void {
  hwnd = CreateWindow(L"hiroWidget", L"", WS_CHILD, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  update();
}

auto pCanvas::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pCanvas::minimumSize() const -> Size {
  if(auto& icon = state().icon) return {(int)icon.width(), (int)icon.height()};
  return {0, 0};
}

auto pCanvas::setAlignment(Alignment) -> void {
  update();
}

auto pCanvas::setColor(Color color) -> void {
  update();
}

auto pCanvas::setDroppable(bool droppable) -> void {
  DragAcceptFiles(hwnd, droppable);
}

auto pCanvas::setFocusable(bool focusable) -> void {
  //handled by windowProc()
}

auto pCanvas::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry(geometry);
  update();
}

auto pCanvas::setGradient(Gradient gradient) -> void {
  update();
}

auto pCanvas::setIcon(const image& icon, Color padding) -> void {
  update();
}

auto pCanvas::update() -> void {
  _rasterize();
  _redraw();
}

//

auto pCanvas::doMouseLeave() -> void {
  return self().doMouseLeave();
}

auto pCanvas::doMouseMove(int x, int y) -> void {
  return self().doMouseMove({x, y});
}

auto pCanvas::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_DROPFILES) {
    if(auto paths = DropPaths(wparam)) self().doDrop(paths);
    return false;
  }

  if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP) {
    if(self().focusable()) return true;
  }

  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_ERASEBKGND || msg == WM_PAINT) {
    _paint();
    return msg == WM_ERASEBKGND;
  }

  if(msg == WM_LBUTTONDOWN) {
    if(self().focusable()) setFocused();
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

//

auto pCanvas::_paint() -> void {
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);

  int sx = 0, sy = 0, dx = 0, dy = 0;
  int width = this->width;
  int height = this->height;
  auto geometry = self().geometry();
  auto alignment = state().alignment ? state().alignment : Alignment{0.5, 0.5};

  if(width <= geometry.width()) {
    sx = 0;
    dx = (geometry.width() - width) * alignment.horizontal();
  } else {
    sx = (width - geometry.width()) * alignment.horizontal();
    dx = 0;
    width = geometry.width();
  }

  if(height <= geometry.height()) {
    sy = 0;
    dy = (geometry.height() - height) * alignment.vertical();
  } else {
    sy = (height - geometry.height()) * alignment.vertical();
    dy = 0;
    height = geometry.height();
  }

  HDC hdc = CreateCompatibleDC(ps.hdc);
  BITMAPINFO bmi{};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;  //GDI stores bitmaps upside now; negative height flips bitmap
  bmi.bmiHeader.biSizeImage = width * height * sizeof(uint32_t);
  void* bits = nullptr;
  HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, nullptr, 0);
  if(bits) {
    for(uint y : range(height)) {
      auto source = (const uint8_t*)pixels.data() + (sy + y) * this->width * sizeof(uint32_t) + sx * sizeof(uint32_t);
      auto target = (uint8_t*)bits + y * width * sizeof(uint32_t);
      for(uint x : range(width)) {
        target[0] = (source[0] * source[3]) / 255;
        target[1] = (source[1] * source[3]) / 255;
        target[2] = (source[2] * source[3]) / 255;
        target[3] = (source[3]);
        source += 4, target += 4;
      }
    }
  }
  SelectObject(hdc, bitmap);

  RECT rc;
  GetClientRect(hwnd, &rc);
  Color padding = state().color;
  if(padding.alpha() == 0) {  //GDI doesn't support real alpha fill
    DrawThemeParentBackground(hwnd, ps.hdc, &rc);
  } else {
    HBRUSH hbr = CreateSolidBrush(RGB(padding.red(), padding.green(), padding.blue()));
    FillRect(ps.hdc, &rc, hbr);
  }

  BLENDFUNCTION bf{AC_SRC_OVER, 0, (BYTE)255, AC_SRC_ALPHA};
  AlphaBlend(ps.hdc, dx, dy, width, height, hdc, 0, 0, width, height, bf);

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

  pixels.reset();
  pixels.resize(width * height);

  if(auto& icon = state().icon) {
    memory::copy<uint32_t>(pixels.data(), icon.data(), width * height);
  } else if(auto& gradient = state().gradient) {
    auto& colors = gradient.state.colors;
    image fill;
    fill.allocate(width, height);
    fill.gradient(colors[0].value(), colors[1].value(), colors[2].value(), colors[3].value());
    memory::copy(pixels.data(), fill.data(), fill.size());
  } else {
    uint32_t color = state().color.value();
    for(auto& pixel : pixels) pixel = color;
  }
}

auto pCanvas::_redraw() -> void {
  InvalidateRect(hwnd, 0, false);
}

}

#endif
