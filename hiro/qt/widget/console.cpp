namespace phoenix {

void pConsole::print(string text) {
}

void pConsole::reset() {
}

void pConsole::setBackgroundColor(Color color) {
  QPalette palette = qtConsole->palette();
  palette.setColor(QPalette::Base, QColor(color.red, color.green, color.blue));
  qtConsole->setPalette(palette);
  qtConsole->setAutoFillBackground(true);
}

void pConsole::setForegroundColor(Color color) {
  QPalette palette = qtConsole->palette();
  palette.setColor(QPalette::Text, QColor(color.red, color.green, color.blue));
  qtConsole->setPalette(palette);
}

void pConsole::setPrompt(string prompt) {
}

void pConsole::constructor() {
  qtWidget = qtConsole = new QtConsole(*this);

  pWidget::synchronizeState();
}

void pConsole::destructor() {
  delete qtConsole;
  qtWidget = qtConsole = nullptr;
}

void pConsole::orphan() {
  destructor();
  constructor();
}

void pConsole::keyPressEvent(QKeyEvent* event) {
}

void pConsole::QtConsole::keyPressEvent(QKeyEvent* event) {
  self.keyPressEvent(event);
}

void pConsole::QtConsole::keyPressEventAcknowledge(QKeyEvent* event) {
  QTextEdit::keyPressEvent(event);
}

}
