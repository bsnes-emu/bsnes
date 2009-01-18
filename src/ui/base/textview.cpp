uintptr_t TextViewWindow::close(event_t) {
  hide();
  return false;
}

void TextViewWindow::setup() {
  create(Window::AutoCenter, 415, 450);
  set_icon(48, 48, (uint32_t*)resource::icon48);

  text.create(Editbox::Multiline | Editbox::Readonly | Editbox::HorizontalScrollNever | Editbox::VerticalScrollAuto, 405, 410);
  ok.create(0, 100, 25, translate["{{textview}}Ok"]);

  attach(text, 5, 5);
  attach(ok, 310, 420);

  on_close = ok.on_tick = bind(&TextViewWindow::close, this);
}
