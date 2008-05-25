uintptr_t MessageWindow::close(Event) {
  hide();
  return false;
}

void MessageWindow::setup() {
  create(Window::AutoCenter, 400, 100, "");
  message.create(0, 390, 60, "");
  ok.create(0, 100, 25, translate["Ok"]);
  attach(message, 5, 5);
  attach(ok, 295, 70);

  on_close = ok.on_tick = bind(&MessageWindow::close, this);
}

void MessageWindow::show(const char *message_, const char *title_) {
  message.set_text(message_);
  set_text(translate[title_]);
  focus();
}
