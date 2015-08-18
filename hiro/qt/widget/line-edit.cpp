#if defined(Hiro_LineEdit)

namespace hiro {

auto pLineEdit::construct() -> void {
  qtWidget = qtLineEdit = new QtLineEdit(*this);
  qtLineEdit->connect(qtLineEdit, SIGNAL(returnPressed()), SLOT(onActivate()));
  qtLineEdit->connect(qtLineEdit, SIGNAL(textEdited(const QString&)), SLOT(onChange()));

  pWidget::construct();
  _setState();
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
  _setState();
}

auto pLineEdit::setEditable(bool editable) -> void {
  _setState();
}

auto pLineEdit::setForegroundColor(Color color) -> void {
  _setState();
}

auto pLineEdit::setText(const string& text) -> void {
  _setState();
}

auto pLineEdit::_setState() -> void {
  if(auto color = state().backgroundColor) {
    QPalette palette = qtLineEdit->palette();
    palette.setColor(QPalette::Base, QColor(color.red(), color.green(), color.blue()));
    qtLineEdit->setPalette(palette);
    qtLineEdit->setAutoFillBackground(true);
  } else {
    //todo
  }
  qtLineEdit->setReadOnly(!state().editable);
  if(auto color = state().foregroundColor) {
    QPalette palette = qtLineEdit->palette();
    palette.setColor(QPalette::Text, QColor(color.red(), color.green(), color.blue()));
    qtLineEdit->setPalette(palette);
  } else {
    //todo
  }
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
