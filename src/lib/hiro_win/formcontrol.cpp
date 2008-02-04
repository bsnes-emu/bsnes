void pFormControl::resize(uint width, uint height) {
  SetWindowPos(hwnd, 0, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
}

void pFormControl::focus() {
  SetFocus(hwnd);
}

bool pFormControl::focused() {
  return true; //fixme
}

void pFormControl::enable(bool state) {
  EnableWindow(hwnd, state);
}

void pFormControl::disable() {
  enable(false);
}

bool pFormControl::enabled() {
  return !(GetWindowLong(hwnd, GWL_STYLE) & WS_DISABLED);
}

uintptr_t pFormControl::handle() {
  return (uintptr_t)hwnd;
}

pFormControl::pFormControl(FormControl &self_) : pWidget(self_), self(self_) {
  hwnd = 0;
}
