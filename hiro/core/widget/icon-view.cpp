#if defined(Hiro_IconView)

auto mIconView::allocate() -> pObject* {
  return new pIconView(*this);
}

auto mIconView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mIconView::append(sIconViewItem item) -> type& {
  state.items.append(item);
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mIconView::backgroundColor() const -> Color {
  return state.backgroundColor;
}

auto mIconView::batchable() const -> bool {
  return state.batchable;
}

auto mIconView::batched() const -> vector<IconViewItem> {
  vector<IconViewItem> items;
  for(auto& item : state.items) {
    if(item->selected()) items.append(item);
  }
  return items;
}

auto mIconView::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mIconView::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mIconView::doContext() const -> void {
  if(state.onContext) return state.onContext();
}

auto mIconView::flow() const -> Orientation {
  return state.flow;
}

auto mIconView::foregroundColor() const -> Color {
  return state.foregroundColor;
}

auto mIconView::item(unsigned position) const -> IconViewItem {
  if(position < itemCount()) return state.items[position];
  return {};
}

auto mIconView::itemCount() const -> unsigned {
  return state.items.size();
}

auto mIconView::items() const -> vector<IconViewItem> {
  vector<IconViewItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
}

auto mIconView::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mIconView::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mIconView::onContext(const function<void ()>& callback) -> type& {
  state.onContext = callback;
  return *this;
}

auto mIconView::orientation() const -> Orientation {
  return state.orientation;
}

auto mIconView::remove(sIconViewItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mIconView::reset() -> type& {
  signal(reset);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  return *this;
}

auto mIconView::selected() const -> IconViewItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mIconView::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mIconView::setBatchable(bool batchable) -> type& {
  state.batchable = batchable;
  signal(setBatchable, batchable);
  return *this;
}

auto mIconView::setFlow(Orientation flow) -> type& {
  state.flow = flow;
  signal(setFlow, flow);
  return *this;
}

auto mIconView::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mIconView::setOrientation(Orientation orientation) -> type& {
  state.orientation = orientation;
  signal(setOrientation, orientation);
  return *this;
}

auto mIconView::setParent(mObject* parent, signed offset) -> type& {
  for(auto n : rrange(state.items)) state.items[n]->destruct();
  mObject::setParent(parent, offset);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

auto mIconView::setSelected(const vector<signed>& selections) -> type& {
  bool selectAll = selections(0, 0) == ~0;
  for(auto& item : state.items) item->state.selected = selectAll;
  if(selectAll) return signal(setItemSelectedAll), *this;
  if(!selections) return signal(setItemSelectedNone), *this;
  for(auto& position : selections) {
    if(position >= itemCount()) continue;
    state.items[position]->state.selected = true;
  }
  signal(setItemSelected, selections);
  return *this;
}

#endif
