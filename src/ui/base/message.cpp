uintptr_t MessageWindow::close(Event) {
  hide();
  return false;
}

void MessageWindow::setup() {
  create(Window::AutoCenter, 400, 100, "");
  message.create(0, 390, 60, "Test\nMessage");
  ok.create(0, 100, 30, "Ok");
  attach(message, 5, 5);
  attach(ok, 295, 65);

  on_close = ok.on_tick = bind(&MessageWindow::close, this);
}

void MessageWindow::show(const char *message_, const char *title_) {
  message.set_text(message_);
  set_text(title_);
  focus();
}
