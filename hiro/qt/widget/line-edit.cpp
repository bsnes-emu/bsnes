#if defined(Hiro_LineEdit)

namespace hiro {

auto pLineEdit::construct() -> void {
  qtWidget = qtLineEdit = new QtLineEdit(*this);
  qtLineEdit->connect(qtLineEdit, SIGNAL(returnPressed()), SLOT(onActivate()));
  qtLineEdit->connect(qtLineEdit, SIGNAL(textEdited(const QString&)), SLOT(onChange()));

  pWidget::construct();
  setBackgroundColor(state().backgroundColor);
  setEditable(state().editable);
  setForegroundColor(state().foregroundColor);
  setText(state().text);
}

auto pLineEdit::destruct() -> void {
  delete qtLineEdit;
  qtWidget = qtLineEdit = nullptr;
}

auto pLineEdit::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text);
  return {size.width() + 12, size.height() + 12};
}

auto pLineEdit::setBackgroundColor(Color color) -> void {
  static auto defaultColor = qtLineEdit->palette().color(QPalette::Base);

  auto palette = qtLineEdit->palette();
  palette.setColor(QPalette::Base, CreateColor(color, defaultColor));
  qtLineEdit->setPalette(palette);
  qtLineEdit->setAutoFillBackground((bool)color);
}

auto pLineEdit::setEditable(bool editable) -> void {
  qtLineEdit->setReadOnly(!state().editable);
}

auto pLineEdit::setForegroundColor(Color color) -> void {
  static auto defaultColor = qtLineEdit->palette().color(QPalette::Text);

  auto palette = qtLineEdit->palette();
  palette.setColor(QPalette::Text, CreateColor(color, defaultColor));
  qtLineEdit->setPalette(palette);
}

auto pLineEdit::setText(const string& text) -> void {
  qtLineEdit->setText(QString::fromUtf8(state().text));
}

auto QtLineEdit::onActivate() -> void {
  p.self().doActivate();
}

auto QtLineEdit::onChange() -> void {
  p.state().text = text().toUtf8().constData();
  p.self().doChange();
}

}

#endif
