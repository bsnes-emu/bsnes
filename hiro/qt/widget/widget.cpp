#if defined(Hiro_Widget)

namespace hiro {

static auto ParentContainer(mObject& object) -> QWidget* {
  if(auto frame = object.parentFrame()) {
    if(auto self = frame->self()) return self->qtFrame;
  }
  if(auto tabFrameItem = object.parentTabFrameItem()) {
    if(auto self = tabFrameItem->self()) return self->qtTabFrameItem;
  }
  if(auto window = object.parentWindow()) {
    if(auto self = window->self()) return self->qtContainer;
  }
  if(auto parent = object.parent()) {
    return ParentContainer(*parent);
  }
  return nullptr;
}

auto pWidget::construct() -> void {
  if(!qtWidget) return;

  if(auto container = ParentContainer(self())) {
    qtWidget->setParent(container);
  }

  setDroppable(self().droppable());
  setEnabled(self().enabled(true));
  setFocusable(self().focusable());
  setFont(self().font(true));
  setMouseCursor(self().mouseCursor());
  setToolTip(self().toolTip());
  setVisible(self().visible(true));
}

auto pWidget::destruct() -> void {
}

auto pWidget::focused() const -> bool {
  if(!qtWidget) return false;
  return qtWidget->hasFocus();
}

auto pWidget::setDroppable(bool droppable) -> void {
  //virtual overload, implemented on a per-widget basis
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(!qtWidget) return;
  qtWidget->setEnabled(enabled);
}

auto pWidget::setFocusable(bool focusable) -> void {
  //virtual overload, implemented on a per-widget basis
}

auto pWidget::setFocused() -> void {
  if(!qtWidget) return;
  qtWidget->setFocus(Qt::OtherFocusReason);
}

auto pWidget::setFont(const Font& font) -> void {
  if(!qtWidget) return;
  qtWidget->setFont(pFont::create(font));
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  if(!qtWidget) return;
  qtWidget->setGeometry(geometry.x(), geometry.y(), geometry.width(), geometry.height());
  pSizable::setGeometry(geometry);
}

auto pWidget::setMouseCursor(const MouseCursor& mouseCursor) -> void {
  auto cursorID = Qt::ArrowCursor;
  if(mouseCursor.name() == MouseCursor::Hand) cursorID = Qt::PointingHandCursor;
  if(mouseCursor.name() == MouseCursor::HorizontalResize) cursorID = Qt::SizeHorCursor;
  if(mouseCursor.name() == MouseCursor::VerticalResize) cursorID = Qt::SizeVerCursor;
  qtWidget->setCursor(cursorID);
}

auto pWidget::setToolTip(const string& toolTip) -> void {
  if(!qtWidget) return;
  qtWidget->setToolTip(QString::fromUtf8(toolTip));
}

auto pWidget::setVisible(bool visible) -> void {
  if(!qtWidget) return;
  qtWidget->setVisible(visible);
}

}

#endif
