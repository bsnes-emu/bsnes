auto mTabFrameItem::allocate() -> pObject* {
  return new pTabFrameItem(*this);
}

auto mTabFrameItem::destruct() -> void {
  if(auto& layout = state.layout) layout->destruct();
  mObject::destruct();
}

//

auto mTabFrameItem::append(sLayout layout) -> type& {
  if(auto& layout = state.layout) remove(layout);
  state.layout = layout;
  layout->setParent(this, 0);
  return *this;
}

auto mTabFrameItem::closable() const -> bool {
  return state.closable;
}

auto mTabFrameItem::icon() const -> image {
  return state.icon;
}

auto mTabFrameItem::layout() const -> sLayout {
  return state.layout;
}

auto mTabFrameItem::movable() const -> bool {
  return state.movable;
}

auto mTabFrameItem::remove() -> type& {
  if(auto tabFrame = parentTabFrame()) tabFrame->remove(*this);
  return *this;
}

auto mTabFrameItem::remove(sLayout layout) -> type& {
  layout->setParent();
  state.layout.reset();
  return *this;
}

auto mTabFrameItem::reset() -> type& {
  if(auto layout = state.layout) remove(layout);
  return *this;
}

auto mTabFrameItem::selected() const -> bool {
  if(auto tabFrame = parentTabFrame()) return offset() == tabFrame->state.selected;
  return false;
}

auto mTabFrameItem::setClosable(bool closable) -> type& {
  state.closable = closable;
  signal(setClosable, closable);
  return *this;
}

auto mTabFrameItem::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mTabFrameItem::setMovable(bool movable) -> type& {
  state.movable = movable;
  signal(setMovable, movable);
  return *this;
}

auto mTabFrameItem::setParent(mObject* parent, signed offset) -> type& {
  if(auto layout = state.layout) layout->destruct();
  mObject::setParent(parent, offset);
  if(auto layout = state.layout) layout->setParent(this, layout->offset());
  return *this;
}

auto mTabFrameItem::setSelected() -> type& {
  if(auto tabFrame = parentTabFrame()) tabFrame->state.selected = offset();
  signal(setSelected);
  return *this;
}

auto mTabFrameItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTabFrameItem::text() const -> string {
  return state.text;
}
