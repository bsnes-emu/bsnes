namespace phoenix {

void pTextEdit::setBackgroundColor(Color color) {
  QPalette palette = qtTextEdit->palette();
  palette.setColor(QPalette::Base, QColor(color.red, color.green, color.blue));
  qtTextEdit->setPalette(palette);
  qtTextEdit->setAutoFillBackground(true);
}

void pTextEdit::setCursorPosition(unsigned position) {
  QTextCursor cursor = qtTextEdit->textCursor();
  unsigned lastCharacter = strlen(qtTextEdit->toPlainText().toUtf8().constData());
  cursor.setPosition(min(position, lastCharacter));
  qtTextEdit->setTextCursor(cursor);
}

void pTextEdit::setEditable(bool editable) {
  qtTextEdit->setTextInteractionFlags(editable ? Qt::TextEditorInteraction : Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
}

void pTextEdit::setForegroundColor(Color color) {
  QPalette palette = qtTextEdit->palette();
  palette.setColor(QPalette::Text, QColor(color.red, color.green, color.blue));
  qtTextEdit->setPalette(palette);
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
