#if defined(Hiro_TextEdit)

namespace hiro {

auto pTextEdit::construct() -> void {
  qtWidget = qtTextEdit = new QtTextEdit(*this);
  qtTextEdit->connect(qtTextEdit, SIGNAL(textChanged()), SLOT(onChange()));

  pWidget::construct();
  _setState();
}

auto pTextEdit::destruct() -> void {
  delete qtTextEdit;
  qtWidget = qtTextEdit = nullptr;
}

auto pTextEdit::setBackgroundColor(Color color) -> void {
  _setState();
}

auto pTextEdit::setCursorPosition(unsigned position) -> void {
  _setState();
}

auto pTextEdit::setEditable(bool editable) -> void {
  _setState();
}

auto pTextEdit::setForegroundColor(Color color) -> void {
  _setState();
}

auto pTextEdit::setText(const string& text) -> void {
  qtTextEdit->setPlainText(QString::fromUtf8(text));
}

auto pTextEdit::setWordWrap(bool wordWrap) -> void {
  _setState();
}

auto pTextEdit::text() const -> string {
  return qtTextEdit->toPlainText().toUtf8().constData();
}

auto pTextEdit::_setState() -> void {
  if(auto color = state().backgroundColor) {
    QPalette palette = qtTextEdit->palette();
    palette.setColor(QPalette::Base, QColor(color.red(), color.green(), color.blue()));
    qtTextEdit->setPalette(palette);
    qtTextEdit->setAutoFillBackground(true);
  } else {
    //todo
  }
  QTextCursor cursor = qtTextEdit->textCursor();
  unsigned lastCharacter = strlen(qtTextEdit->toPlainText().toUtf8().constData());
  cursor.setPosition(min(state().cursorPosition, lastCharacter));
  qtTextEdit->setTextCursor(cursor);
  qtTextEdit->setTextInteractionFlags(state().editable
    ? Qt::TextEditorInteraction
    : Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse
  );
  if(auto color = state().foregroundColor) {
    QPalette palette = qtTextEdit->palette();
    palette.setColor(QPalette::Text, QColor(color.red(), color.green(), color.blue()));
    qtTextEdit->setPalette(palette);
  } else {
    //todo
  }
  qtTextEdit->setWordWrapMode(state().wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);
  qtTextEdit->setHorizontalScrollBarPolicy(state().wordWrap ? Qt::ScrollBarAlwaysOff : Qt::ScrollBarAlwaysOn);
  qtTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

auto QtTextEdit::onChange() -> void {
//p.state().text = text();
  p.self().doChange();
}

}

#endif
