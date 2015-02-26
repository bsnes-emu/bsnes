MessageWindow::MessageWindow(const string& text) {
  state.text = text;
}

auto MessageWindow::error(Buttons buttons) -> Response {
  state.buttons = buttons;
  return pMessageWindow::error(state);
}

auto MessageWindow::information(Buttons buttons) -> Response {
  state.buttons = buttons;
  return pMessageWindow::information(state);
}

auto MessageWindow::question(Buttons buttons) -> Response {
  state.buttons = buttons;
  return pMessageWindow::question(state);
}

auto MessageWindow::setParent(shared_pointer<mWindow> parent) -> type& {
  state.parent = parent;
  return *this;
}

auto MessageWindow::setText(const string& text) -> type& {
  state.text = text;
  return *this;
}

auto MessageWindow::setTitle(const string& title) -> type& {
  state.title = title;
  return *this;
}

auto MessageWindow::warning(MessageWindow::Buttons buttons) -> Response {
  state.buttons = buttons;
  return pMessageWindow::warning(state);
}
