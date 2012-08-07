static MessageWindow::Response MessageWindow_response(MessageWindow::Buttons buttons, UINT response) {
  if(response == IDOK) return MessageWindow::Response::Ok;
  if(response == IDCANCEL) return MessageWindow::Response::Cancel;
  if(response == IDYES) return MessageWindow::Response::Yes;
  if(response == IDNO) return MessageWindow::Response::No;
  if(buttons == MessageWindow::Buttons::OkCancel) return MessageWindow::Response::Cancel;
  if(buttons == MessageWindow::Buttons::YesNo) return MessageWindow::Response::No;
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONINFORMATION;
  if(buttons == MessageWindow::Buttons::Ok) flags |= MB_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::none() ? parent.p.hwnd : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response pMessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONQUESTION;
  if(buttons == MessageWindow::Buttons::Ok) flags |= MB_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::none() ? parent.p.hwnd : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response pMessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONWARNING;
  if(buttons == MessageWindow::Buttons::Ok) flags |= MB_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::none() ? parent.p.hwnd : 0, utf16_t(text), L"", flags));
}

MessageWindow::Response pMessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  UINT flags = MB_ICONERROR;
  if(buttons == MessageWindow::Buttons::Ok) flags |= MB_OK;
  if(buttons == MessageWindow::Buttons::OkCancel) flags |= MB_OKCANCEL;
  if(buttons == MessageWindow::Buttons::YesNo) flags |= MB_YESNO;
  return MessageWindow_response(buttons, MessageBox(&parent != &Window::none() ? parent.p.hwnd : 0, utf16_t(text), L"", flags));
}
