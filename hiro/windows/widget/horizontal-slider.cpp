#if defined(Hiro_HorizontalSlider)

namespace hiro {

auto pHorizontalSlider::construct() -> void {
  hwnd = CreateWindow(
    TRACKBAR_CLASS, L"", WS_CHILD | WS_TABSTOP | TBS_TRANSPARENTBKGND | TBS_NOTICKS | TBS_BOTH | TBS_HORZ,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0
  );
  pWidget::construct();
  setLength(state().length);
  setPosition(state().position);
}

auto pHorizontalSlider::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pHorizontalSlider::minimumSize() const -> Size {
  return {0, 25};
}

auto pHorizontalSlider::setLength(unsigned length) -> void {
  length += (length == 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
}

auto pHorizontalSlider::setPosition(unsigned position) -> void {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)position);
}

auto pHorizontalSlider::onChange() -> void {
  unsigned position = SendMessage(hwnd, TBM_GETPOS, 0, 0);
  if(position == state().position) return;
  state().position = position;
  self().doChange();
}

}

#endif
