#if defined(Hiro_MessageWindow)

namespace hiro {

static auto MessageWindow_buttons(MessageWindow::Buttons buttons) -> QMessageBox::StandardButtons {
  QMessageBox::StandardButtons standardButtons = QMessageBox::NoButton;
  if(buttons == MessageWindow::Buttons::Ok) standardButtons = QMessageBox::Ok;
  if(buttons == MessageWindow::Buttons::OkCancel) standardButtons = QMessageBox::Ok | QMessageBox::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) standardButtons = QMessageBox::Yes | QMessageBox::No;
  if(buttons == MessageWindow::Buttons::YesNoCancel) standardButtons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
  return standardButtons;
}

static auto MessageWindow_response(MessageWindow::Buttons buttons, QMessageBox::StandardButton response) -> MessageWindow::Response {
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

auto pMessageWindow::error(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_response(
    state.buttons, QMessageBox::critical(state.parent ? state.parent->self()->qtWindow : nullptr, state.title ? QString::fromUtf8(state.title) : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

auto pMessageWindow::information(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_response(
    state.buttons, QMessageBox::information(state.parent ? state.parent->self()->qtWindow : nullptr, state.title ? QString::fromUtf8(state.title) : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

auto pMessageWindow::question(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_response(
    state.buttons, QMessageBox::question(state.parent ? state.parent->self()->qtWindow : nullptr, state.title ? QString::fromUtf8(state.title) : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

auto pMessageWindow::warning(MessageWindow::State& state) -> MessageWindow::Response {
  return MessageWindow_response(
    state.buttons, QMessageBox::warning(state.parent ? state.parent->self()->qtWindow : nullptr, state.title ? QString::fromUtf8(state.title) : " ",
    QString::fromUtf8(state.text), MessageWindow_buttons(state.buttons))
  );
}

}

#endif
