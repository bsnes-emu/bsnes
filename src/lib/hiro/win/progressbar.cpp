void pProgressbar::create(unsigned style, unsigned width, unsigned height) {
  hwnd = CreateWindow(PROGRESS_CLASS, L"", WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
}

unsigned pProgressbar::get_progress() {
  unsigned progress = SendMessage(hwnd, PBM_GETPOS, 0, 0);
  return max(0U, min(progress, 100U));
}

void pProgressbar::set_progress(unsigned progress) {
  progress = max(0U, min(progress, 100U));
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)progress, 0);
}

pProgressbar::pProgressbar(Progressbar &self_) : pFormControl(self_), self(self_) {
}
