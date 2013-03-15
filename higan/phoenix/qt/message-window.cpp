namespace phoenix {

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

  //MessageWindow was closed via window manager, rather than by a button; assume a cancel/no response
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::information(&parent != &Window::none() ? parent.p.qtWindow : nullptr, " ",
    QString::fromUtf8(text), MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response pMessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::question(&parent != &Window::none() ? parent.p.qtWindow : nullptr, " ",
    QString::fromUtf8(text), MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response pMessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::warning(&parent != &Window::none() ? parent.p.qtWindow : nullptr, " ",
    QString::fromUtf8(text), MessageWindow_buttons(buttons))
  );
}

MessageWindow::Response pMessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow_response(
    buttons, QMessageBox::critical(&parent != &Window::none() ? parent.p.qtWindow : nullptr, " ",
    QString::fromUtf8(text), MessageWindow_buttons(buttons))
  );
}

}
