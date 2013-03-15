namespace phoenix {

MessageWindow::Response pMessageWindow::information(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::question(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::warning(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::critical(Window &parent, const string &text, MessageWindow::Buttons buttons) {
  return MessageWindow::Response::Ok;
}

}
