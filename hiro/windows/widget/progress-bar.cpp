#if defined(Hiro_ProgressBar)

namespace hiro {

auto pProgressBar::construct() -> void {
  hwnd = CreateWindow(PROGRESS_CLASS, L"",
    WS_CHILD | PBS_SMOOTH,
    0, 0, 0, 0, _parentHandle(), nullptr, GetModuleHandle(0), 0);
  pWidget::construct();
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
  setPosition(state().position);
}

auto pProgressBar::destruct() -> void {
  DestroyWindow(hwnd);
}

auto pProgressBar::minimumSize() const -> Size {
  return {0, 23};
}

auto pProgressBar::setPosition(unsigned position) -> void {
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)position, 0);
}

}

#endif
