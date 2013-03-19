namespace phoenix {

struct pMessageWindow {
  static MessageWindow::Response error(MessageWindow::State &state);
  static MessageWindow::Response information(MessageWindow::State &state);
  static MessageWindow::Response question(MessageWindow::State &state);
  static MessageWindow::Response warning(MessageWindow::State &state);
};

}
