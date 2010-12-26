void HorizontalSlider::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height, unsigned length) {
  length += (length == 0);
  widget->window = CreateWindow(
    TRACKBAR_CLASS, L"",
    WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH | TBS_HORZ,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SetWindowLongPtr(widget->window, GWLP_USERDATA, (LONG_PTR)this);
  SendMessage(widget->window, TBM_SETRANGE, (WPARAM)true, (LPARAM)MAKELONG(0, length - 1));
  SendMessage(widget->window, TBM_SETPAGESIZE, 0, (LPARAM)(length >> 3));
  setPosition(0);
}

unsigned HorizontalSlider::position() {
  return SendMessage(widget->window, TBM_GETPOS, 0, 0);
}

void HorizontalSlider::setPosition(unsigned position) {
  SendMessage(widget->window, TBM_SETPOS, (WPARAM)true, (LPARAM)(horizontalSlider->position = position));
}

HorizontalSlider::HorizontalSlider() {
  horizontalSlider = new HorizontalSlider::Data;
}
