#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  qtWidget = qtLabel = new QLabel;

  pWidget::construct();
  setAlignment(state().alignment);
  setBackgroundColor(state().backgroundColor);
  setForegroundColor(state().foregroundColor);
  setText(state().text);
}

auto pLabel::destruct() -> void {
  delete qtLabel;
  qtWidget = qtLabel = nullptr;
}

auto pLabel::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text);
  return {size.width(), size.height()};
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  if(!alignment) alignment = {0.0, 0.5};
  qtLabel->setAlignment((Qt::Alignment)CalculateAlignment(alignment));
}

auto pLabel::setBackgroundColor(Color color) -> void {
  static auto defaultColor = qtLabel->palette().color(QPalette::Base);

  auto palette = qtLabel->palette();
  palette.setColor(QPalette::Base, CreateColor(color, defaultColor));
  qtLabel->setPalette(palette);
  qtLabel->setAutoFillBackground((bool)color);
}

auto pLabel::setForegroundColor(Color color) -> void {
  static auto defaultColor = qtLabel->palette().color(QPalette::Text);

  auto palette = qtLabel->palette();
  palette.setColor(QPalette::Text, CreateColor(color, defaultColor));
  qtLabel->setPalette(palette);
}

auto pLabel::setText(const string& text) -> void {
  qtLabel->setText(QString::fromUtf8(text));
}

}

#endif
