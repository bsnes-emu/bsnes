static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, UINT response) {
  if(response == IDOK) return MessageWindow::Response::Ok;
  if(response == IDCANCEL) return MessageWindow::Response::Cancel;
  if(response == IDYES) return MessageWindow::Response::Yes;
  if(response == IDNO) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  return MessageWindow::Response::Ok;
}

MessageWindow::Response MessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONINFORMATION;
  if(buttons == Buttons::Ok) flags |= MB_OK;
  if(buttons == Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::None ? parent.window->window : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response MessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONQUESTION;
  if(buttons == Buttons::Ok) flags |= MB_OK;
  if(buttons == Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::None ? parent.window->window : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response MessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONWARNING;
  if(buttons == Buttons::Ok) flags |= MB_OK;
  if(buttons == Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::None ? parent.window->window : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response MessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONERROR;
  if(buttons == Buttons::Ok) flags |= MB_OK;
  if(buttons == Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::None ? parent.window->window : 0, utf16_t(text), L"", flags));
}
