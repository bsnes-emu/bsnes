namespace phoenix {

static QMessageBox::StandardButtons MessageWindow_buttons(MessageWindow::Buttons buttons) {
  QMessageBox::StandardButtons standardButtons = QMessageBox::NoButton;
  if(buttons == MessageWindow::Buttons::Ok) standardButtons = QMessageBox::Ok;
  if(buttons == MessageWindow::Buttons::OkCancel) standardButtons = QMessageBox::Ok | QMessageBox::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) standardButtons = QMessageBox::Yes | QMessageBox::No;
  if(buttons == MessageWindow::Buttons::YesNoCancel) standardButtons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
  return standardButtons;
}

static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, QMessageBox::StandardButton response) {
  if(response == QMessageBox::Ok) return MessageWindow::Response::Ok;
  if(response == QMessageBox::Cancel) return MessageWindow::Response::Cancel;
  if(response == QMessageBox::Yes) return MessageWindow::Response::Yes;
  if(response == QMessageBox::No) return MessageWindow::Response::No;

  //MessageWindow was closed via window manager, rather than by a button; assume a cancel/no response
  if(buttons == MessageWindow::Buttons::Ok) return MessageWindow::Response::Ok;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::YesNoCancel) return MessageWindow::Response::Cancel;

  throw;
}

MessageWindow::Response pMessageWindow::error(MessageWindow::State& state) {
  return MessageWindow_response(
    state.buttons, QMessageBox::critical(state.parent ? state.parent->p.qtWindow : nullptr, state.title ? state.title : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

MessageWindow::Response pMessageWindow::information(MessageWindow::State& state) {
  return MessageWindow_response(
    state.buttons, QMessageBox::information(state.parent ? state.parent->p.qtWindow : nullptr, state.title ? state.title : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

MessageWindow::Response pMessageWindow::question(MessageWindow::State& state) {
  return MessageWindow_response(
    state.buttons, QMessageBox::question(state.parent ? state.parent->p.qtWindow : nullptr, state.title ? state.title : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

MessageWindow::Response pMessageWindow::warning(MessageWindow::State& state) {
  return MessageWindow_response(
    state.buttons, QMessageBox::warning(state.parent ? state.parent->p.qtWindow : nullptr, state.title ? state.title : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

}
