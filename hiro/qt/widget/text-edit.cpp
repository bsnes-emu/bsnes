#if defined(Hiro_TextEdit)

namespace hiro {

auto pTextEdit::construct() -> void {
  qtWidget = qtTextEdit = new QtTextEdit(*this);
  qtTextEdit->connect(qtTextEdit, SIGNAL(textChanged()), SLOT(onChange()));

  pWidget::construct();
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);
  _setState();
}

auto pTextEdit::destruct() -> void {
if(Application::state().quit) return;  //TODO: hack
  delete qtTextEdit;
  qtWidget = qtTextEdit = nullptr;
}

auto pTextEdit::setBackgroundColor(Color color) -> void {
  static auto defaultColor = qtTextEdit->palette().color(QPalette::Base);

  auto palette = qtTextEdit->palette();
  palette.setColor(QPalette::Base, CreateColor(color, defaultColor));
  qtTextEdit->setPalette(palette);
  qtTextEdit->setAutoFillBackground((bool)color);
}

auto pTextEdit::setEditable(bool editable) -> void {
  _setState();
}

auto pTextEdit::setForegroundColor(Color color) -> void {
  static auto defaultColor = qtTextEdit->palette().color(QPalette::Text);

  auto palette = qtTextEdit->palette();
  palette.setColor(QPalette::Text, CreateColor(color, defaultColor));
  qtTextEdit->setPalette(palette);
}

auto pTextEdit::setText(const string& text) -> void {
  qtTextEdit->setPlainText(QString::fromUtf8(text));
}

auto pTextEdit::setTextCursor(TextCursor cursor) -> void {
  _setState();
}

auto pTextEdit::setWordWrap(bool wordWrap) -> void {
  _setState();
}

auto pTextEdit::text() const -> string {
  return qtTextEdit->toPlainText().toUtf8().constData();
}

auto pTextEdit::textCursor() const -> TextCursor {
  //TODO
  return state().textCursor;
}

auto pTextEdit::_setState() -> void {
  QTextCursor cursor = qtTextEdit->textCursor();
  signed lastCharacter = strlen(qtTextEdit->toPlainText().toUtf8().constData());
  cursor.setPosition(max(0, min(lastCharacter, state().textCursor.offset())));
  cursor.setPosition(max(0, min(lastCharacter, state().textCursor.offset() + state().textCursor.length())), QTextCursor::KeepAnchor);
  qtTextEdit->setTextCursor(cursor);
  qtTextEdit->setTextInteractionFlags(state().editable
    ? Qt::TextEditorInteraction
    : Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse
  );
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
