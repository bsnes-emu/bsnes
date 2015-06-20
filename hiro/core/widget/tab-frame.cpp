#if defined(Hiro_TabFrame)

auto mTabFrame::allocate() -> pObject* {
  return new pTabFrame(*this);
}

auto mTabFrame::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mTabFrame::append(sTabFrameItem item) -> type& {
  state.items.append(item);
  item->setParent(this, items() - 1);
  signal(append, item);
  return *this;
}

auto mTabFrame::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mTabFrame::doClose(sTabFrameItem item) const -> void {
  if(state.onClose) return state.onClose(item);
}

auto mTabFrame::doMove(sTabFrameItem from, sTabFrameItem to) const -> void {
  if(state.onMove) return state.onMove(from, to);
}

auto mTabFrame::edge() const -> Edge {
  return state.edge;
}

auto mTabFrame::item(unsigned position) const -> TabFrameItem {
  if(position < items()) return state.items[position];
  return {};
}

auto mTabFrame::items() const -> unsigned {
  return state.items.size();
}

auto mTabFrame::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mTabFrame::onClose(const function<void (TabFrameItem)>& callback) -> type& {
  state.onClose = callback;
  return *this;
}

auto mTabFrame::onMove(const function<void (TabFrameItem, TabFrameItem)>& callback) -> type& {
  state.onMove = callback;
  return *this;
}

auto mTabFrame::remove(sTabFrameItem item) -> type& {
  auto offset = item->offset();
  item->setParent();
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(offset, items())) {
    state.items[n]->adjustOffset(-1);
  }
  return *this;
}

auto mTabFrame::reset() -> type& {
  while(state.items) remove(state.items.last());
  return *this;
}

auto mTabFrame::selected() const -> TabFrameItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mTabFrame::setEdge(Edge edge) -> type& {
  state.edge = edge;
  signal(setEdge, edge);
  return *this;
}

auto mTabFrame::setParent(mObject* parent, signed offset) -> type& {
  for(auto& item : state.items) item->destruct();
  mObject::setParent(parent, offset);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

#endif
