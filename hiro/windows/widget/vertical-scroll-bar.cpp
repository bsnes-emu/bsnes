#if defined(Hiro_VerticalScrollBar)

namespace hiro {

auto pVerticalScrollBar::construct() -> void {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | SBS_VERT,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  setLength(state().length);
  setPosition(state().position);
}

auto pVerticalScrollBar::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pVerticalScrollBar::minimumSize() const -> Size {
  return {18_sx, 0};
}

auto pVerticalScrollBar::setLength(unsigned length) -> void {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
}

auto pVerticalScrollBar::setPosition(unsigned position) -> void {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

auto pVerticalScrollBar::onChange(WPARAM wparam) -> void {
  unsigned position = ScrollEvent(hwnd, wparam);
  if(position == state().position) return;
  state().position = position;
  self().doChange();
}

}

#endif
