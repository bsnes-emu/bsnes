static QMessageBox::StandardButtons MessageWindow_buttons(MessageWindow::Buttons buttons) {
  QMessageBox::StandardButtons standardButtons = QMessageBox::NoButton;
  if(buttons == MessageWindow::Buttons::Ok) standardButtons = QMessageBox::Ok;
  if(buttons == MessageWindow::Buttons::OkCancel) standardButtons = QMessageBox::Ok | QMessageBox::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) standardButtons = QMessageBox::Yes | QMessageBox::No;
  return standardButtons;
}

static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, QMessageBox::StandardButton response) {
  if(response == QMessageBox::Ok) return MessageWindow::Response::Ok;
  if(response == QMessageBox::Cancel) return MessageWindow::Response::Cancel;
  if(response == QMessageBox::Yes) return MessageWindow::Response::Yes;
  if(response == QMessageBox::No) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  return MessageWindow::Response::Ok;
}

MessageWindow::Response MessageWindow::information(Window &parent, const char *text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::information(&parent != &Window::None ? parent.window : 0, " ", text, MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response MessageWindow::question(Window &parent, const char *text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::question(&parent != &Window::None ? parent.window : 0, " ", text, MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response MessageWindow::warning(Window &parent, const char *text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::warning(&parent != &Window::None ? parent.window : 0, " ", text, MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response MessageWindow::critical(Window &parent, const char *text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::critical(&parent != &Window::None ? parent.window : 0, " ", text, MessageWindow_buttons(buttons))
  );
}
