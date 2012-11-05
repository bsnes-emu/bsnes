Geometry pProgressBar::minimumGeometry() {
  return { 0, 0, 0, 23 };
}

void pProgressBar::setPosition(unsigned position) {
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)position, 0);
}

void pProgressBar::constructor() {
  hwnd = CreateWindow(PROGRESS_CLASS, L"", WS_CHILD | PBS_SMOOTH, 0, 0, 0, 0, parentWindow->p.hwnd, (HMENU)id, GetModuleHandle(0), 0);
  SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&progressBar);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
  setPosition(progressBar.state.position);
  synchronize();
}

void pProgressBar::destructor() {
  DestroyWindow(hwnd);
}

void pProgressBar::orphan() {
  destructor();
  constructor();
}
