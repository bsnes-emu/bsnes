#if defined(Hiro_VerticalSlider)

namespace hiro {

auto pVerticalSlider::construct() -> void {
  hwnd = CreateWindow(
    TRACKBAR_CLASS, L"", WS_CHILD | WS_TABSTOP | TBS_TRANSPARENTBKGND | TBS_NOTICKS | TBS_BOTH | TBS_VERT,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  setLength(state().length);
  setPosition(state().position);
}

auto pVerticalSlider::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pVerticalSlider::minimumSize() const -> Size {
  return {25, 0};
}

auto pVerticalSlider::setLength(unsigned length) -> void {
  length += (length == 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
}

auto pVerticalSlider::setPosition(unsigned position) -> void {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

auto pVerticalSlider::onChange() -> void {
  unsigned position = SendMessage(hwnd, TBM_GETPOS, 0, 0);
  if(position == state().position) return;
  state().position = position;
  self().doChange();
}

}

#endif
