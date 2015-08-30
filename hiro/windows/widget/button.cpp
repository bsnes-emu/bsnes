#if defined(Hiro_Button)

namespace hiro {

static auto Button_paintProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam,
  bool bordered, bool checked, bool enabled, const Font& font, const Image& image, Orientation orientation, const string& text
) -> LRESULT {
  if(msg == WM_PAINT) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    auto state = Button_GetState(hwnd);
    Button_CustomDraw(hwnd, ps,
      (state & BST_PUSHED || checked) ? PBS_PRESSED
    : (state & BST_HOT) ? PBS_HOT
    : bordered ? (enabled ? PBS_NORMAL : PBS_DISABLED)
    : 0, font, image, orientation, text
    );
    EndPaint(hwnd, &ps);
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

//BUTTON cannot draw borderless buttons on its own
//BS_OWNERDRAW will send WM_DRAWITEM; but will disable hot-tracking notifications
//to gain hot-tracking plus borderless buttons; BUTTON is superclassed and WM_PAINT is hijacked
//note: letting hiro paint bordered buttons will lose the fade animations on Vista+;
//however, it will allow placing icons immediately next to text (original forces icon left alignment)
static auto CALLBACK Button_windowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT {
  if(auto object = (mObject*)GetWindowLongPtr(hwnd, GWLP_USERDATA)) {
    if(auto button = dynamic_cast<mButton*>(object)) {
      if(auto self = button->self()) {
        if(msg == WM_ERASEBKGND) return DefWindowProc(hwnd, msg, wparam, lparam);
        if(msg == WM_PAINT) return Button_paintProc(hwnd, msg, wparam, lparam,
          button->state.bordered, false, button->enabled(true), button->font(true),
          button->state.image, button->state.orientation, button->state.text
        );
        return self->windowProc(hwnd, msg, wparam, lparam);
      }
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

auto pButton::construct() -> void {
  hwnd = CreateWindow(
    L"BUTTON", L"", WS_CHILD | WS_TABSTOP,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  windowProc = (WindowProc)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
  SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)Button_windowProc);
  pWidget::_setState();
  _setState();
}

auto pButton::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pButton::minimumSize() const -> Size {
  Size image = state().image.size();
  Size text = state().text ? pFont::size(self().font(true), state().text) : Size{};
  Size size;
  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(image.width() + (image && text ? 5 : 0) + text.width());
    size.setHeight(max(image.height(), text.height()));
  }
  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(image.width(), text.width()));
    size.setHeight(image.height() + (image && text ? 5 : 0) + text.height());
  }
  size.setHeight(max(size.height(), pFont::size(self().font(true), " ").height()));
  return {size.width() + (state().bordered && text ? 20 : 10), size.height() + 10};
}

auto pButton::setBordered(bool bordered) -> void {
  _setState();
}

auto pButton::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  _setState();
}

auto pButton::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  _setState();
}

auto pButton::setImage(const Image& image) -> void {
  _setState();
}

auto pButton::setOrientation(Orientation orientation) -> void {
  _setState();
}

auto pButton::setText(const string& text) -> void {
  _setState();
}

auto pButton::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  _setState();
}

auto pButton::onActivate() -> void {
  self().doActivate();
}

auto pButton::_setState() -> void {
  InvalidateRect(hwnd, 0, false);
}

//this function is designed to be used with Button, CheckButton, and RadioButton
auto Button_CustomDraw(HWND hwnd, PAINTSTRUCT& ps, unsigned state, const Font& font, const Image& image, Orientation orientation, const string& text) -> void {
  RECT rc;
  GetClientRect(hwnd, &rc);
  Geometry geometry{rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top}, imageGeometry, textGeometry;
  if(image) imageGeometry.setSize(image.size());
  if(text) textGeometry.setSize(pFont::size(font, text));

  Position position;
  Size size;

  switch(orientation) {
  case Orientation::Horizontal:
    size = {imageGeometry.width() + (image && text ? 5 : 0) + textGeometry.width(), max(imageGeometry.height(), textGeometry.height())};
    position = {(geometry.width() - size.width()) / 2, (geometry.height() - size.height()) / 2};
    imageGeometry.setPosition({position.x(), position.y() + (size.height() - imageGeometry.height()) / 2});
    textGeometry.setPosition({position.x() + size.width() - textGeometry.width(), position.y() + (size.height() - textGeometry.height()) / 2});
    break;
  case Orientation::Vertical:
    size = {max(imageGeometry.width(), textGeometry.width()), imageGeometry.height() + (image && text ? 5 : 0) + textGeometry.height()};
    position = {(geometry.width() - size.width()) / 2, (geometry.height() - size.height()) / 2};
    imageGeometry.setPosition({position.x() + (size.width() - imageGeometry.width()) / 2, position.y()});
    textGeometry.setPosition({position.x() + (size.width() - textGeometry.width()) / 2, position.y() + size.height() - textGeometry.height()});
    break;
  }

  HDC hdcSource = CreateCompatibleDC(ps.hdc);
  DrawThemeParentBackground(hwnd, ps.hdc, &rc);

  if(state) {
    if(auto theme = OpenThemeData(hwnd, L"BUTTON")) {
      DrawThemeBackground(theme, ps.hdc, BP_PUSHBUTTON, state, &rc, &ps.rcPaint);
      CloseThemeData(theme);
    }
  }

  if(GetFocus() == hwnd) {
    signed offset = state ? 4 : 1;
    RECT rcFocus{rc.left + offset, rc.top + offset, rc.right - offset, rc.bottom - offset};
    if(!state || state == PBS_NORMAL) DrawFocusRect(ps.hdc, &rcFocus);
  }

  if(image) {
    auto bitmap = CreateBitmap(image);
    SelectBitmap(hdcSource, bitmap);
    BLENDFUNCTION blend{AC_SRC_OVER, 0, (BYTE)(IsWindowEnabled(hwnd) ? 255 : 128), AC_SRC_ALPHA};
    AlphaBlend(
      ps.hdc, imageGeometry.x(), imageGeometry.y(), image.width(), image.height(),
      hdcSource, 0, 0, image.width(), image.height(), blend
    );
    DeleteObject(bitmap);
  }

  if(text) {
    utf16_t wText(text);
    SetBkMode(ps.hdc, TRANSPARENT);
    SetTextColor(ps.hdc, GetSysColor(IsWindowEnabled(hwnd) ? COLOR_BTNTEXT : COLOR_GRAYTEXT));
    auto hFont = pFont::create(font);
    SelectObject(ps.hdc, hFont);
    RECT rcText{textGeometry.x(), textGeometry.y(), textGeometry.x() + textGeometry.width(), textGeometry.y() + textGeometry.height()};
    DrawText(ps.hdc, wText, -1, &rcText, DT_NOPREFIX | DT_END_ELLIPSIS);
    DeleteObject(hFont);
  }

  DeleteDC(hdcSource);
}

}

#endif
