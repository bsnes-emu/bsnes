Geometry pProgressBar::minimumGeometry() {
  return { 0, 0, 0, 23 };
}

void pProgressBar::setPosition(unsigned position) {
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)position, 0);
}

void pProgressBar::constructor() {
  setParent(Window::None);
}

void pProgressBar::setParent(Window &parent) {
  if(hwnd) DestroyWindow(hwnd);
  hwnd = CreateWindow(PROGRESS_CLASS, L"", WS_CHILD | WS_VISIBLE | PBS_SMOOTH, 0, 0, 0, 0, parent.p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&progressBar);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
  setPosition(progressBar.state.position);
}
