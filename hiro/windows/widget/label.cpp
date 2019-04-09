#if defined(Hiro_Label)

namespace hiro {

//warning: WS_CLIPSIBLINGS flag will prevent Label widgets from rendering inside of Frame widgets
auto pLabel::construct() -> void {
  hwnd = CreateWindow(L"hiroWidget", L"", WS_CHILD, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setText(state().text);
}

auto pLabel::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pLabel::minimumSize() const -> Size {
  auto size = pFont::size(self().font(true), state().text ? state().text : " ");
  return {size.width(), size.height()};
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  InvalidateRect(hwnd, 0, false);
}

auto pLabel::setBackgroundColor(Color color) -> void {
  InvalidateRect(hwnd, 0, false);
}

auto pLabel::setForegroundColor(Color color) -> void {
  InvalidateRect(hwnd, 0, false);
}

auto pLabel::setText(const string& text) -> void {
  InvalidateRect(hwnd, 0, false);
}

auto pLabel::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  if(msg == WM_GETDLGCODE) {
    return DLGC_STATIC | DLGC_WANTCHARS;
  }

  if(msg == WM_ERASEBKGND || msg == WM_PAINT) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    RECT rc;
    GetClientRect(hwnd, &rc);

    auto hdcMemory = CreateCompatibleDC(ps.hdc);
    auto hbmMemory = CreateCompatibleBitmap(ps.hdc, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdcMemory, hbmMemory);

    if(auto color = state().backgroundColor) {
      auto brush = CreateSolidBrush(CreateRGB(color));
      FillRect(hdcMemory, &rc, brush);
      DeleteObject(brush);
    } else if(self().parentTabFrame(true)) {
      DrawThemeParentBackground(hwnd, hdcMemory, &rc);
    } else if(auto window = self().parentWindow(true)) {
      if(auto color = window->backgroundColor()) {
        auto brush = CreateSolidBrush(CreateRGB(color));
        FillRect(hdcMemory, &rc, brush);
        DeleteObject(brush);
      } else {
        DrawThemeParentBackground(hwnd, hdcMemory, &rc);
      }
    }

    utf16_t text(state().text);
    SetBkMode(hdcMemory, TRANSPARENT);
    SelectObject(hdcMemory, hfont);
    DrawText(hdcMemory, text, -1, &rc, DT_CALCRECT | DT_END_ELLIPSIS);
    uint height = rc.bottom;

    GetClientRect(hwnd, &rc);
    rc.top = (rc.bottom - height) / 2;
    rc.bottom = rc.top + height;
    uint horizontalAlignment = DT_CENTER;
    if(state().alignment.horizontal() < 0.333) horizontalAlignment = DT_LEFT;
    if(state().alignment.horizontal() > 0.666) horizontalAlignment = DT_RIGHT;
    uint verticalAlignment = DT_VCENTER;
    if(state().alignment.vertical() < 0.333) verticalAlignment = DT_TOP;
    if(state().alignment.vertical() > 0.666) verticalAlignment = DT_BOTTOM;
    if(auto color = state().foregroundColor) {
      SetTextColor(hdcMemory, CreateRGB(color));
    }
    DrawText(hdcMemory, text, -1, &rc, DT_END_ELLIPSIS | horizontalAlignment | verticalAlignment);

    GetClientRect(hwnd, &rc);
    BitBlt(ps.hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMemory, 0, 0, SRCCOPY);
    DeleteObject(hbmMemory);
    DeleteObject(hdcMemory);
    EndPaint(hwnd, &ps);

    return msg == WM_ERASEBKGND;
  }

  if(msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN) {
    switch(msg) {
    case WM_LBUTTONDOWN: self().doMousePress(Mouse::Button::Left); break;
    case WM_MBUTTONDOWN: self().doMousePress(Mouse::Button::Middle); break;
    case WM_RBUTTONDOWN: self().doMousePress(Mouse::Button::Right); break;
    }
  }

  if(msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP) {
    switch(msg) {
    case WM_LBUTTONUP: self().doMouseRelease(Mouse::Button::Left); break;
    case WM_MBUTTONUP: self().doMouseRelease(Mouse::Button::Middle); break;
    case WM_RBUTTONUP: self().doMouseRelease(Mouse::Button::Right); break;
    }
  }

  return pWidget::windowProc(hwnd, msg, wparam, lparam);
}

}

#endif
