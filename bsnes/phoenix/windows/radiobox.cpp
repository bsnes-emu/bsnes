void RadioBox::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  radioBox->parentWindow = &parent;
  radioBox->parent = this;
  radioBox->parent->radioBox->items.append(this);
  widget->window = CreateWindow(
    L"BUTTON", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_RADIOBUTTON,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(parent.window->defaultFont ? parent.window->defaultFont : os.os->proportionalFont), 0);
  setChecked();
}

void RadioBox::create(RadioBox &parent, unsigned x, unsigned y, unsigned width, unsigned height, const char *text) {
  radioBox->parentWindow = parent.radioBox->parentWindow;
  radioBox->parent = parent.radioBox->parent;
  radioBox->parent->radioBox->items.append(this);
  widget->window = CreateWindow(
    L"BUTTON", utf16_t(text),
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_RADIOBUTTON,
    x, y, width, height,
    GetParent(radioBox->parent->widget->window), (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, WM_SETFONT, (WPARAM)(radioBox->parentWindow->window->defaultFont ? radioBox->parentWindow->window->defaultFont : os.os->proportionalFont), 0);
}

bool RadioBox::checked() {
  return SendMessage(widget->window, BM_GETCHECK, 0, 0);
}

void RadioBox::setChecked() {
  foreach(item, radioBox->parent->radioBox->items) {
    SendMessage(item->widget->window, BM_SETCHECK, (WPARAM)(item == this), 0);
  }
}

RadioBox::RadioBox() {
  radioBox = new RadioBox::Data;
}
