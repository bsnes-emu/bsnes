void CheckBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  widget->window = CreateWindow(
    L"BUTTON", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_CHECKBOX,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : OS::os->proportionalFont), 0);
}

bool CheckBox::checked() {
  return SendMessage(widget->window, BM_GETCHECK, 0, 0);
}

void CheckBox::setChecked(bool checked) {
  SendMessage(widget->window, BM_SETCHECK, (WPARAM)checked, 0);
}
