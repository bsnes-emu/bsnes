namespace phoenix {

MessageWindow::Response pMessageWindow::error(MessageWindow::State &state) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::information(MessageWindow::State &state) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::question(MessageWindow::State &state) {
  return MessageWindow::Response::Ok;
}

MessageWindow::Response pMessageWindow::warning(MessageWindow::State &state) {
  return MessageWindow::Response::Ok;
}

}
