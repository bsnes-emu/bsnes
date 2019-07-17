#if defined(Hiro_Label)

namespace hiro {

auto pLabel::construct() -> void {
  qtWidget = qtLabel = new QtLabel(*this);

  pWidget::construct();
  qtLabel->update();
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
  qtLabel->update();
}

auto pLabel::setBackgroundColor(Color color) -> void {
  qtLabel->update();
}

auto pLabel::setFont(const Font& font) -> void {
  pWidget::setFont(font);
  qtLabel->update();
}

auto pLabel::setForegroundColor(Color color) -> void {
  qtLabel->update();
}

auto pLabel::setText(const string& text) -> void {
  qtLabel->update();
}

auto QtLabel::mousePressEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMousePress(Mouse::Button::Left); break;
  case Qt::MidButton: p.self().doMousePress(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMousePress(Mouse::Button::Right); break;
  }
}

auto QtLabel::mouseReleaseEvent(QMouseEvent* event) -> void {
  switch(event->button()) {
  case Qt::LeftButton: p.self().doMouseRelease(Mouse::Button::Left); break;
  case Qt::MidButton: p.self().doMouseRelease(Mouse::Button::Middle); break;
  case Qt::RightButton: p.self().doMouseRelease(Mouse::Button::Right); break;
  }
}

//QLabel ignores QPalette ... so we have to implement our own Label class atop QWidget ...
auto QtLabel::paintEvent(QPaintEvent* event) -> void {
  QPainter painter(p.qtLabel);
  if(auto& color = p.state().backgroundColor) {
    painter.fillRect(event->rect(), CreateColor(color));
  }
  if(auto& text = p.state().text) {
    if(auto& color = p.state().foregroundColor) {
      QPen pen(CreateColor(color));
      painter.setPen(pen);
    }
    auto alignment = p.state().alignment ? p.state().alignment : Alignment{0.0, 0.5};
    painter.drawText(event->rect(), CalculateAlignment(alignment), QString::fromUtf8(text));
  }
}

}

#endif
