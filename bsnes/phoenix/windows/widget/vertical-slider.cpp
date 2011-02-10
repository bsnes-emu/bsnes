unsigned VerticalSlider::position() {
  return SendMessage(widget->window, TBM_GETPOS, 0, 0);
}

void VerticalSlider::setLength(unsigned length) {
  length += (length == 0);
  SendMessage(widget->window, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(widget->window, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
  setPosition(0);
}

void VerticalSlider::setPosition(unsigned position) {
  SendMessage(widget->window, TBM_SETPOS, (WPARAM)true, (LPARAM)(verticalSlider->position = position));
}

VerticalSlider::VerticalSlider() {
  verticalSlider = new VerticalSlider::Data;

  widget->window = CreateWindow(
    TRACKBAR_CLASS, L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH | TBS_VERT,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
}
