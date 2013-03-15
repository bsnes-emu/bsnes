namespace phoenix {

struct pMessageWindow {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);

  enum class Type : unsigned { Information, Question, Warning, Critical };
  static MessageWindow::Response message(Window &parent, const string &text, MessageWindow::Buttons buttons, Type type);
};

}
