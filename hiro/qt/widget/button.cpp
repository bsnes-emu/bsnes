#if defined(Hiro_Button)

namespace hiro {

auto pButton::construct() -> void {
  qtWidget = qtButton = new QtButton(*this);
  qtButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  qtButton->connect(qtButton, SIGNAL(released()), SLOT(onActivate()));

  setBordered(state().bordered);
  setImage(state().image);
  setOrientation(state().orientation);
  setText(state().text);

  pWidget::construct();
}

auto pButton::destruct() -> void {
  delete qtButton;
  qtWidget = qtButton = nullptr;
}

auto pButton::minimumSize() const -> Size {
  auto size = pFont::size(qtWidget->font(), state().text ? state().text : " ");

  if(state().orientation == Orientation::Horizontal) {
    size.setWidth(size.width() + state().image.width());
    size.setHeight(max(state().image.height(), size.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(state().image.width(), size.width()));
    size.setHeight(size.height() + state().image.height());
  }

  return {size.width() + (state().text ? 20 : 12), size.height() + 12};
}

auto pButton::setBordered(bool bordered) -> void {
  qtButton->setAutoRaise(!bordered);
}

auto pButton::setImage(const Image& image) -> void {
  qtButton->setIconSize(QSize(image.width(), image.height()));
  qtButton->setIcon(CreateImage(image));
  qtButton->setStyleSheet("text-align: top;");
}

auto pButton::setOrientation(Orientation orientation) -> void {
  switch(orientation) {
  case Orientation::Horizontal: qtButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon); break;
  case Orientation::Vertical:   qtButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);  break;
  }
}

auto pButton::setText(const string& text) -> void {
  qtButton->setText(QString::fromUtf8(text));
}

auto QtButton::onActivate() -> void {
  p.self().doActivate();
}

}

#endif
