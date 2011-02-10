unsigned ProgressBar::position() {
  return SendMessage(widget->window, PBM_GETPOS, 0, 0);
}

void ProgressBar::setPosition(unsigned position) {
  SendMessage(widget->window, PBM_SETPOS, (WPARAM)position, 0);
}

ProgressBar::ProgressBar() {
  widget->window = CreateWindow(
    PROGRESS_CLASS, L"",
    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
    0, 0, 64, 64,
    OS::os->nullWindow, (HMENU)object->id, GetModuleHandle(0), 0
  );
  SendMessage(widget->window, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessage(widget->window, PBM_SETSTEP, MAKEWPARAM(1, 0), 0);
}
