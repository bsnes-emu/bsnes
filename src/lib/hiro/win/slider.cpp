void pSlider::create(unsigned style, unsigned width, unsigned height, unsigned length) {
  if(length < 1) length = 1;

  hwnd = CreateWindow(TRACKBAR_CLASS, L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH |
    (style & Slider::Vertical ? TBS_VERT : TBS_HORZ),
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(hwnd, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)0);
}

unsigned pSlider::get_position() {
  return SendMessage(hwnd, TBM_GETPOS, 0, 0);
}

void pSlider::set_position(unsigned position) {
  SendMessage(hwnd, TBM_SETPOS, (WPARAM)true, (LPARAM)(slider_position = position));
}

pSlider::pSlider(Slider &self_) : pFormControl(self_), self(self_) {
  slider_position = 0;
}
