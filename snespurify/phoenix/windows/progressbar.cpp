void ProgressBar::create(Window &parent, unsigned x, unsigned y, unsigned width, unsigned height) {
  widget->window = CreateWindow(
    PROGRESS_CLASS, L"",
    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    x, y, width, height,
    parent.widget->window, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SendMessage(widget->window, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(widget->window, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
}

unsigned ProgressBar::position() {
  return SendMessage(widget->window, PBM_GETPOS, 0, 0);
}

void ProgressBar::setPosition(unsigned position) {
  SendMessage(widget->window, PBM_SETPOS, (WPARAM)position, 0);
}
