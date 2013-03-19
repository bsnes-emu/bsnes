namespace phoenix {

static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, UINT response) {
  if(response == IDOK) return MessageWindow::Response::Ok;
  if(response == IDCANCEL) return MessageWindow::Response::Cancel;
  if(response == IDYES) return MessageWindow::Response::Yes;
  if(response == IDNO) return MessageWindow::Response::No;

  //default responses if window was closed without a button selected
  if(buttons == MessageWindow::Buttons::Ok) return MessageWindow::Response::Ok;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::YesNoCancel) return MessageWindow::Response::Cancel;

  throw;
}

static UINT MessageWindow_buttons(MessageWindow::Buttons buttons) {
  if(buttons == MessageWindow::Buttons::Ok) return MB_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) return MB_OKCANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) return MB_YESNO;
  if(buttons == MessageWindow::Buttons::YesNoCancel) return MB_YESNOCANCEL;
  throw;
}

MessageWindow::Response pMessageWindow::error(MessageWindow::State &state) {
  UINT flags = MB_ICONERROR | MessageWindow_buttons(state.buttons);
  return MessageWindow_response(state.buttons, MessageBox(
    state.parent ? state.parent->p.hwnd : 0, utf16_t(state.text), utf16_t(state.title), flags
  ));
}

MessageWindow::Response pMessageWindow::information(MessageWindow::State &state) {
  UINT flags = MB_ICONINFORMATION | MessageWindow_buttons(state.buttons);
  return MessageWindow_response(state.buttons, MessageBox(
    state.parent ? state.parent->p.hwnd : 0, utf16_t(state.text), utf16_t(state.title), flags
  ));
}

MessageWindow::Response pMessageWindow::question(MessageWindow::State &state) {
  UINT flags = MB_ICONQUESTION | MessageWindow_buttons(state.buttons);
  return MessageWindow_response(state.buttons, MessageBox(
    state.parent ? state.parent->p.hwnd : 0, utf16_t(state.text), utf16_t(state.title), flags
  ));
}

MessageWindow::Response pMessageWindow::warning(MessageWindow::State &state) {
  UINT flags = MB_ICONWARNING | MessageWindow_buttons(state.buttons);
  return MessageWindow_response(state.buttons, MessageBox(
    state.parent ? state.parent->p.hwnd : 0, utf16_t(state.text), utf16_t(state.title), flags
  ));
}

}
