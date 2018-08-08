namespace hiro {

static auto CALLBACK ToolTip_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto toolTip = (pToolTip*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto result = toolTip->windowProc(hwnd, msg, wparam, lparam)) {
      return result();
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

pToolTip::pToolTip(const string& toolTipText) {
  text = toolTipText;

  htheme = OpenThemeData(hwnd, L"TOOLTIP");
  hwnd = CreateWindowEx(
    WS_EX_TOOLWINDOW | WS_EX_TOPMOST | (htheme ? WS_EX_LAYERED : 0), L"hiroToolTip", L"",
    WS_POPUP, 0, 0, 0, 0,
    0, 0, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
  tracking.x = -1;
  tracking.y = -1;

  timeout.setInterval(5000);
  timeout.onActivate([&] { hide(); });
}

pToolTip::~pToolTip() {
  hide();
  if(htheme) { CloseThemeData(htheme); htheme = nullptr; }
  if(hwnd) { DestroyWindow(hwnd); hwnd = nullptr; }
}

auto pToolTip::drawLayered() -> void {
  auto hdcOutput = GetDC(nullptr);

  uint32_t* below = nullptr;
  auto hdcBelow = CreateCompatibleDC(hdcOutput);
  auto hbmBelow = CreateBitmap(hdcBelow, size.cx, size.cy, below);
  SelectObject(hdcBelow, hbmBelow);
  RECT rc{};
  rc.left = 0, rc.top = 0, rc.right = size.cx, rc.bottom = size.cy;
  DrawThemeBackground(htheme, hdcBelow, TTP_STANDARD, TTSS_NORMAL, &rc, nullptr);

  uint32_t* above = nullptr;
  auto hdcAbove = CreateCompatibleDC(hdcOutput);
  auto hbmAbove = CreateBitmap(hdcAbove, size.cx, size.cy, above);
  SelectObject(hdcAbove, hbmAbove);

  memory::copy<uint32_t>(above, below, size.cx * size.cy);

  auto hfont = pFont::create(Font());
  SelectObject(hdcAbove, hfont);
  SetBkMode(hdcAbove, TRANSPARENT);
  SetTextColor(hdcAbove, RGB(0, 0, 0));
  utf16_t drawText(text);
  rc.left += 6, rc.top += 6, rc.right -= 6, rc.bottom -= 6;
  DrawText(hdcAbove, drawText, -1, &rc, DT_LEFT | DT_TOP);
  DeleteObject(hfont);

  for(uint n : range(size.cx * size.cy)) {
    below[n] = (below[n] & 0xff000000) | (above[n] & 0x00ffffff);
  }

  BLENDFUNCTION blend{};
  blend.BlendOp = AC_SRC_OVER;
  blend.SourceConstantAlpha = 255;
  blend.AlphaFormat = AC_SRC_ALPHA;
  POINT zero{};
  zero.x = 0, zero.y = 0;
  UpdateLayeredWindow(hwnd, hdcOutput, &position, &size, hdcBelow, &zero, RGB(0, 0, 0), &blend, ULW_ALPHA);

  DeleteObject(hbmAbove);
  DeleteObject(hbmBelow);
  DeleteDC(hdcAbove);
  DeleteDC(hdcBelow);
  ReleaseDC(nullptr, hdcOutput);
}

auto pToolTip::drawOpaque() -> void {
  PAINTSTRUCT ps;
  BeginPaint(hwnd, &ps);
  RECT rc{};
  GetClientRect(hwnd, &rc);
  auto brush = CreateSolidBrush(RGB(0, 0, 0));
  FillRect(ps.hdc, &rc, brush);
  DeleteObject(brush);
  rc.left += 1, rc.top += 1, rc.right -= 1, rc.bottom -= 1;
  brush = CreateSolidBrush(RGB(255, 255, 225));
  FillRect(ps.hdc, &rc, brush);
  DeleteObject(brush);
  rc.left += 5, rc.top += 5, rc.right -= 5, rc.bottom -= 5;
  SetBkMode(ps.hdc, TRANSPARENT);
  auto font = pFont::create(Font());
  SelectObject(ps.hdc, font);
  SetTextColor(ps.hdc, RGB(0, 0, 0));
  DrawText(ps.hdc, utf16_t(text), -1, &rc, DT_LEFT | DT_TOP);
  DeleteObject(font);
  EndPaint(hwnd, &ps);
}

auto pToolTip::show() -> void {
  if(auto toolTip = pApplication::state().toolTip) {
    if(toolTip != this) toolTip->hide();
  }
  pApplication::state().toolTip = this;

  GetCursorPos(&position);
  if(position.x == tracking.x && position.y == tracking.y) return;
  tracking.x = position.x, tracking.y = position.y;

  position.y += 18;
  auto textSize = pFont::size(Font(), text ? text : " ");
  size.cx = 12 + textSize.width();
  size.cy = 12 + textSize.height();

  //try to keep the tool-tip onscreen
  auto desktop = pDesktop::size();
  if(position.x + size.cx >= desktop.width ()) position.x = desktop.width () - size.cx;
  if(position.y + size.cy >= desktop.height()) position.y = desktop.height() - size.cy;
  if(position.x < 0) position.x = 0;
  if(position.y < 0) position.y = 0;

  SetWindowPos(hwnd, HWND_TOP, position.x, position.y, size.cx, size.cy, SWP_NOACTIVATE | SWP_SHOWWINDOW);
  if(htheme) drawLayered();

  timeout.setEnabled(true);
}

auto pToolTip::hide() -> void {
  pApplication::state().toolTip = nullptr;
  timeout.setEnabled(false);
  ShowWindow(hwnd, SW_HIDE);
  GetCursorPos(&tracking);
}

auto pToolTip::windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> maybe<LRESULT> {
  switch(msg) {
  case WM_ERASEBKGND:
  case WM_PAINT:
    if(htheme) break;
    drawOpaque();
    return msg == WM_ERASEBKGND;
  case WM_MOUSEMOVE:
  case WM_MOUSELEAVE: {
    POINT point{};
    GetCursorPos(&point);
    if(point.x != tracking.x || point.y != tracking.y) hide();
  } break;
  case WM_LBUTTONDOWN: case WM_LBUTTONUP:
  case WM_MBUTTONDOWN: case WM_MBUTTONUP:
  case WM_RBUTTONDOWN: case WM_RBUTTONUP:
    hide();
    break;
  }

  return {};
}

}
