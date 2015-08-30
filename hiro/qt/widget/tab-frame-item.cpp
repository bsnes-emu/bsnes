#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
  qtTabFrameItem = new QWidget;

  if(auto parent = _parent()) {
    parent->qtTabFrame->addTab(qtTabFrameItem, "");
  }

  _setState();
}

auto pTabFrameItem::destruct() -> void {
}

auto pTabFrameItem::append(sLayout layout) -> void {
}

auto pTabFrameItem::remove(sLayout layout) -> void {
}

auto pTabFrameItem::setClosable(bool closable) -> void {
}

auto pTabFrameItem::setGeometry(Geometry geometry) -> void {
  if(auto layout = state().layout) {
    auto offset = qtTabFrameItem->geometry();
    geometry.setPosition({0, 0});
    geometry.setWidth(geometry.width() - (geometry.width() - offset.width()));
    geometry.setHeight(geometry.height() - (geometry.height() - offset.height()));
    layout->setGeometry(geometry);
  }
}

auto pTabFrameItem::setImage(const Image& image) -> void {
  _setState();
}

auto pTabFrameItem::setMovable(bool movable) -> void {
}

auto pTabFrameItem::setSelected() -> void {
  _setState();
}

auto pTabFrameItem::setText(const string& text) -> void {
  _setState();
}

auto pTabFrameItem::setVisible(bool visible) -> void {
  _setState();
}

auto pTabFrameItem::_parent() -> maybe<pTabFrame&> {
  if(auto parent = self().parentTabFrame()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTabFrameItem::_setState() -> void {
  if(auto parent = _parent()) {
    parent->qtTabFrame->setTabIcon(self().offset(), CreateImage(state().image));
    if(state().selected) parent->qtTabFrame->setCurrentIndex(self().offset());
    parent->qtTabFrame->setTabText(self().offset(), QString::fromUtf8(state().text));
    if(auto layout = state().layout) {
      auto geometry = parent->self().geometry();
      auto offset = qtTabFrameItem->geometry();
      geometry.setPosition({0, 0});
      geometry.setWidth(geometry.width() - (geometry.width() - offset.width()));
      geometry.setHeight(geometry.height() - (geometry.height() - offset.height()));
      layout->setGeometry(geometry);
      layout->setVisible(layout->visible(true));
    }
  }
}

}

#endif
