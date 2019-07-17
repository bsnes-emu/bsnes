#if defined(Hiro_TabFrame)

auto mTabFrameItem::allocate() -> pObject* {
  return new pTabFrameItem(*this);
}

auto mTabFrameItem::destruct() -> void {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::destruct();
}

//

auto mTabFrameItem::append(sSizable sizable) -> type& {
  if(auto& sizable = state.sizable) remove(sizable);
  state.sizable = sizable;
  sizable->setParent(this, 0);
  signal(append, sizable);
  return *this;
}

auto mTabFrameItem::closable() const -> bool {
  return state.closable;
}

auto mTabFrameItem::icon() const -> image {
  return state.icon;
}

auto mTabFrameItem::movable() const -> bool {
  return state.movable;
}

auto mTabFrameItem::remove() -> type& {
  if(auto tabFrame = parentTabFrame()) tabFrame->remove(*this);
  return *this;
}

auto mTabFrameItem::remove(sSizable sizable) -> type& {
  signal(remove, sizable);
  state.sizable.reset();
  sizable->setParent();
  return *this;
}

auto mTabFrameItem::reset() -> type& {
  if(auto& sizable = state.sizable) remove(sizable);
  return *this;
}

auto mTabFrameItem::selected() const -> bool {
  return state.selected;
}

auto mTabFrameItem::setClosable(bool closable) -> type& {
  state.closable = closable;
  signal(setClosable, closable);
  return *this;
}

auto mTabFrameItem::setEnabled(bool enabled) -> type& {
  mObject::setEnabled(enabled);
  if(auto& sizable = state.sizable) sizable->setEnabled(sizable->enabled());
  return *this;
}

auto mTabFrameItem::setFont(const Font& font) -> type& {
  mObject::setFont(font);
  if(auto& sizable = state.sizable) sizable->setFont(sizable->font());
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

auto mTabFrameItem::setParent(mObject* parent, int offset) -> type& {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::setParent(parent, offset);
  if(auto& sizable = state.sizable) sizable->setParent(this, sizable->offset());
  return *this;
}

auto mTabFrameItem::setSelected() -> type& {
  if(auto parent = parentTabFrame()) {
    for(auto& item : parent->state.items) item->state.selected = false;
  }
  state.selected = true;
  signal(setSelected);
  return *this;
}

auto mTabFrameItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTabFrameItem::setVisible(bool visible) -> type& {
  mObject::setVisible(visible);
  if(auto& sizable = state.sizable) sizable->setVisible(sizable->visible());
  return *this;
}

auto mTabFrameItem::sizable() const -> Sizable {
  return state.sizable;
}

auto mTabFrameItem::text() const -> string {
  return state.text;
}

#endif
