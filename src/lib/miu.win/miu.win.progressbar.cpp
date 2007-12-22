void pProgressbar::create(uint style, uint width, uint height) {
  hwnd = CreateWindow(PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    0, 0, width, height,
    miu().p.default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(hwnd, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
}

uint pProgressbar::get_progress() {
uint progress = SendMessage(hwnd, PBM_GETPOS, 0, 0);
  return max(0, min(progress, 100));
}

void pProgressbar::set_progress(uint progress) {
  progress = max(0, min(progress, 100));
  SendMessage(hwnd, PBM_SETPOS, (WPARAM)progress, 0);
}

pProgressbar::pProgressbar(Progressbar &self_) : pFormControl(self_), self(self_) {
}
