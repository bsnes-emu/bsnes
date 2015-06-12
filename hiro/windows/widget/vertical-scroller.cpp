#if defined(Hiro_VerticalScroller)

namespace hiro {

auto pVerticalScroller::construct() -> void {
  hwnd = CreateWindow(
    L"SCROLLBAR", L"", WS_CHILD | SBS_VERT,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&reference);
  pWidget::_setState();
  setLength(state().length);
  setPosition(state().position);
}

auto pVerticalScroller::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pVerticalScroller::minimumSize() const -> Size {
  return {18, 0};
}

auto pVerticalScroller::setLength(unsigned length) -> void {
  length += (length == 0);
  SetScrollRange(hwnd, SB_CTL, 0, length - 1, TRUE);
}

auto pVerticalScroller::setPosition(unsigned position) -> void {
  SetScrollPos(hwnd, SB_CTL, position, TRUE);
}

auto pVerticalScroller::onChange(WPARAM wparam) -> void {
  unsigned position = ScrollEvent(hwnd, wparam);
  if(position == state().position) return;
  state().position = position;
  self().doChange();
}

}

#endif
