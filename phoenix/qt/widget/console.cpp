namespace phoenix {

void pConsole::print(string text) {
}

void pConsole::reset() {
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
