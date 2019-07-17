#if defined(Hiro_Button)

namespace hiro {

auto pButton::construct() -> void {
  qtWidget = qtButton = new QtButton(*this);
  qtButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
  qtButton->connect(qtButton, SIGNAL(released()), SLOT(onActivate()));

  setBordered(state().bordered);
  setIcon(state().icon);
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
    size.setWidth(size.width() + state().icon.width());
    size.setHeight(max(size.height(), state().icon.height()));
  }

  if(state().orientation == Orientation::Vertical) {
    size.setWidth(max(size.width(), state().icon.width()));
    size.setHeight(size.height() + state().icon.height());
  }

  return {size.width() + (state().text ? 20 : 12), size.height() + 12};
}

auto pButton::setBordered(bool bordered) -> void {
  qtButton->setAutoRaise(!bordered);
}

auto pButton::setIcon(const image& icon) -> void {
  qtButton->setIconSize(QSize(icon.width(), icon.height()));
  qtButton->setIcon(CreateIcon(icon));
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
