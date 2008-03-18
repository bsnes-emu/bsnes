void pCheckbox::create(uint style, uint width, uint height, const char *text) {
  hwnd = CreateWindow("BUTTON", text ? text : "", WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_CHECKBOX,
    0, 0, width, height,
    phiro().default_hwnd, (HMENU)instance, GetModuleHandle(0), 0);
  SendMessage(hwnd, WM_SETFONT, (WPARAM)phiro().default_font, 0);
}

void pCheckbox::set_text(const char *text) {
  SetWindowText(hwnd, text ? text : "");
}

void pCheckbox::check(bool state) {
bool prev = checked();
  SendMessage(hwnd, BM_SETCHECK, (WPARAM)(state ? TRUE : FALSE), 0);
  if(prev != state) {
    if(self.on_tick) self.on_tick(Event(Event::Tick, state, &self));
  }
}

void pCheckbox::uncheck() {
  check(false);
}

bool pCheckbox::checked() {
  return SendMessage(hwnd, BM_GETCHECK, 0, 0);
}

pCheckbox::pCheckbox(Checkbox &self_) : pFormControl(self_), self(self_) {
}
