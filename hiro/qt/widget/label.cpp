#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  qtWidget = qtLabel = new QLabel;

  setAlignment(state().alignment);
  setText(state().text);

  pWidget::construct();
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

auto pLabel::setText(const string& text) -> void {
  qtLabel->setText(QString::fromUtf8(text));
}

}

#endif
