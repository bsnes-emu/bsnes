namespace hiro {

struct pMessageWindow {
  static auto error(MessageWindow::State& state) -> MessageWindow::Response;
  static auto information(MessageWindow::State& state) -> MessageWindow::Response;
  static auto question(MessageWindow::State& state) -> MessageWindow::Response;
  static auto warning(MessageWindow::State& state) -> MessageWindow::Response;
};

}
