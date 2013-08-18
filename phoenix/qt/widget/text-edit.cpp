namespace phoenix {

void pTextEdit::setCursorPosition(unsigned position) {
  QTextCursor cursor = qtTextEdit->textCursor();
  unsigned lastCharacter = strlen(qtTextEdit->toPlainText().toUtf8().constData());
  cursor.setPosition(min(position, lastCharacter));
  qtTextEdit->setTextCursor(cursor);
}

void pTextEdit::setEditable(bool editable) {
  qtTextEdit->setReadOnly(!editable);
}

void pTextEdit::setText(string text) {
  qtTextEdit->setPlainText(QString::fromUtf8(text));
}

void pTextEdit::setWordWrap(bool wordWrap) {
  qtTextEdit->setWordWrapMode(wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);
  qtTextEdit->setHorizontalScrollBarPolicy(wordWrap ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAlwaysOn);
  qtTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

string pTextEdit::text() {
  return qtTextEdit->toPlainText().toUtf8().constData();
}

void pTextEdit::constructor() {
  qtWidget = qtTextEdit = new QTextEdit;
  connect(qtTextEdit, SIGNAL(textChanged()), SLOT(onChange()));

  pWidget::synchronizeState();
  setEditable(textEdit.state.editable);
  setText(textEdit.state.text);
  setWordWrap(textEdit.state.wordWrap);
}

void pTextEdit::destructor() {
  if(sizable.state.layout) sizable.state.layout->remove(textEdit);
  delete qtTextEdit;
  qtWidget = qtTextEdit = nullptr;
}

void pTextEdit::orphan() {
  destructor();
  constructor();
}

void pTextEdit::onChange() {
  textEdit.state.text = text();
  if(textEdit.onChange) textEdit.onChange();
}

}
