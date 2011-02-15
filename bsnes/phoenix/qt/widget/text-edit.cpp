void pTextEdit::setCursorPosition(unsigned position) {
  QTextCursor cursor = qtTextEdit->textCursor();
  unsigned lastCharacter = strlen(qtTextEdit->toPlainText().toUtf8().constData());
  cursor.setPosition(min(position, lastCharacter));
  qtTextEdit->setTextCursor(cursor);
}

void pTextEdit::setEditable(bool editable) {
  qtTextEdit->setReadOnly(!editable);
}

void pTextEdit::setText(const string &text) {
  qtTextEdit->setPlainText(QString::fromUtf8(text));
}

void pTextEdit::setWordWrap(bool wordWrap) {
  qtTextEdit->setWordWrapMode(wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);
}

string pTextEdit::text() {
  return qtTextEdit->toPlainText().toUtf8().constData();
}

pTextEdit::pTextEdit(TextEdit &textEdit) : textEdit(textEdit), pWidget(textEdit) {
  qtWidget = qtTextEdit = new QTextEdit;
  connect(qtTextEdit, SIGNAL(textChanged()), SLOT(onChange()));
}

void pTextEdit::onChange() {
  textEdit.state.text = text();
  if(textEdit.onChange) textEdit.onChange();
}
