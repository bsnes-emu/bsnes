#if defined(Hiro_Frame)

namespace hiro {

auto pFrame::construct() -> void {
  hwnd = CreateWindowEx(
    //WS_EX_TRANSPARENT fixes rendering issues caused by Windows using WS_CLIPCHILDREN
    WS_EX_TRANSPARENT, L"BUTTON", L"",
    WS_CHILD | BS_GROUPBOX,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setText(state().text);
}

auto pFrame::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pFrame::append(sSizable sizable) -> void {
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto& sizable = state().sizable) sizable->setEnabled(sizable->enabled());
  pWidget::setEnabled(enabled);
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  bool empty = !state().text;
  auto size = pFont::size(hfont, state().text);
  //offsets are based on the default Windows 10 theme
  pWidget::setGeometry({
    geometry.x(),
    geometry.y() - (empty ? 6_sy : 3_sy),
    geometry.width(),
    geometry.height() + (empty ? 7_sy : 4_sy)
  });
  if(auto& sizable = state().sizable) {
    sizable->setGeometry({
      geometry.x() + 5_sx,
      geometry.y() + (empty ? 5_sy : size.height()),
      geometry.width() - 10_sx,
      geometry.height() - (empty ? 10_sy : size.height() + 5_sy)
    });
  }
}

auto pFrame::setText(const string& text) -> void {
  SetWindowText(hwnd, utf16_t(text));
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto& sizable = state().sizable) sizable->setVisible(sizable->visible());
  pWidget::setVisible(visible);
}

}

#endif
