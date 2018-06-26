#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  hwnd = CreateWindow(L"hiroLabel", L"",
    WS_CHILD,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
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
  SetWindowText(hwnd, utf16_t(text));
  InvalidateRect(hwnd, 0, false);
}

static auto CALLBACK Label_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  auto label = (mLabel*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if(!label) return DefWindowProc(hwnd, msg, wparam, lparam);
  auto window = label->parentWindow(true);
  if(!window) return DefWindowProc(hwnd, msg, wparam, lparam);

  switch(msg) {
  case WM_GETDLGCODE: return DLGC_STATIC | DLGC_WANTCHARS;
  case WM_ERASEBKGND:
  case WM_PAINT: {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    RECT rc;
    GetClientRect(hwnd, &rc);

    auto hdcMemory = CreateCompatibleDC(ps.hdc);
    auto hbmMemory = CreateCompatibleBitmap(ps.hdc, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdcMemory, hbmMemory);

    uint length = GetWindowTextLength(hwnd);
    wchar_t text[length + 1];
    GetWindowText(hwnd, text, length + 1);
    text[length] = 0;

    //todo: use DrawThemeParentBackground if Label is inside TabFrame
    if(auto color = label->backgroundColor()) {
      auto brush = CreateSolidBrush(CreateRGB(color));
      FillRect(hdcMemory, &rc, brush);
      DeleteObject(brush);
    } else if(auto brush = window->self()->hbrush) {
      FillRect(hdcMemory, &rc, brush);
    } else {
      DrawThemeParentBackground(hwnd, hdcMemory, &rc);
    }

    SetBkMode(hdcMemory, TRANSPARENT);
    SelectObject(hdcMemory, label->self()->hfont);
    DrawText(hdcMemory, text, -1, &rc, DT_CALCRECT | DT_END_ELLIPSIS);
    uint height = rc.bottom;

    GetClientRect(hwnd, &rc);
    rc.top = (rc.bottom - height) / 2;
    rc.bottom = rc.top + height;
    uint horizontalAlignment = DT_CENTER;
    if(label->alignment().horizontal() < 0.333) horizontalAlignment = DT_LEFT;
    if(label->alignment().horizontal() > 0.666) horizontalAlignment = DT_RIGHT;
    uint verticalAlignment = DT_VCENTER;
    if(label->alignment().vertical() < 0.333) verticalAlignment = DT_TOP;
    if(label->alignment().vertical() > 0.666) verticalAlignment = DT_BOTTOM;
    if(auto color = label->foregroundColor()) {
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
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

}

#endif
