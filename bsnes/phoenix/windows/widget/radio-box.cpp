void RadioBox::setParent(RadioBox &parent) {
  radioBox->parent = parent.radioBox->parent;
  radioBox->parent->radioBox->items.append(this);
  parent.setChecked();
}

void RadioBox::setText(const string &text) {
  SetWindowText(widget->window, utf16_t(text));
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

  radioBox->parent = this;
  radioBox->parent->radioBox->items.append(this);
  widget->window = CreateWindow(
    L"BUTTON", L"",
    WS_CHILD | WS_TABSTOP | WS_VISIBLE | BS_RADIOBUTTON,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  setChecked();
}
