#if defined(Hiro_HorizontalSlider)

namespace hiro {

auto pHorizontalSlider::construct() -> void {
  //TBS_TRANSPARENTBKGND is needed to render the transparent area of sliders properly inside TabFrame controls
  //however, this flag will prevent the slider control from redrawing during vertical window resizes when not inside TabFrame controls
  //this is because WM_PRINTCLIENT must be implemented in the parent window for this case
  //however, WM_PRINTCLIENT is incompatible with WM_PAINT, which is how most hiro custom widgets are rendered
  //as a hacky workaround, TBS_TRANSPARENTBKGND is enabled only when sliders are placed inside of TabFrame controls
  auto style = WS_CHILD | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH | TBS_HORZ;
  if(self().parentTabFrame(true)) style |= TBS_TRANSPARENTBKGND;
  hwnd = CreateWindow(TRACKBAR_CLASS, L"", style, 0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  setLength(state().length);
  setPosition(state().position);
}

auto pHorizontalSlider::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pHorizontalSlider::minimumSize() const -> Size {
  return {0, 25_sy};
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
