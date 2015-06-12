#if defined(Hiro_HorizontalScroller)

namespace hiro {

auto pHorizontalScroller::construct() -> void {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | WS_TABSTOP | SBS_HORZ,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  setLength(state().length);
  setPosition(state().position);
}

auto pHorizontalScroller::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pHorizontalScroller::minimumSize() const -> Size {
  return {0, 18};
}

auto pHorizontalScroller::setLength(unsigned length) -> void {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
}

auto pHorizontalScroller::setPosition(unsigned position) -> void {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

auto pHorizontalScroller::onChange(WPARAM wparam) -> void {
  unsigned position = ScrollEvent(hwnd, wparam);
  if(position == state().position) return;
  state().position = position;
  self().doChange();
}

}

#endif
