auto mIconView::allocate() -> pObject* {
  return new pIconView(*this);
}

auto mIconView::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mIconView::append(shared_pointer<mIconViewItem> item) -> type& {
  state.items.append(item);
  item->setParent(this, items() - 1);
  signal(append, item);
  return *this;
}

auto mIconView::backgroundColor() const -> Color {
  return state.backgroundColor;
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

auto mIconView::item(unsigned position) const -> shared_pointer<mIconViewItem> {
  if(position >= items()) throw;
  return state.items[position];
}

auto mIconView::items() const -> unsigned {
  return state.items.size();
}

auto mIconView::multiSelect() const -> bool {
  return state.multiSelect;
}

auto mIconView::onActivate(const function<void ()>& function) -> type& {
  state.onActivate = function;
  return *this;
}

auto mIconView::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mIconView::onContext(const function<void ()>& function) -> type& {
  state.onContext = function;
  return *this;
}

auto mIconView::orientation() const -> Orientation {
  return state.orientation;
}

auto mIconView::remove(shared_pointer<mIconViewItem> item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), items())) {
    state.items[n]->offset(-1);
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

auto mIconView::selected() const -> maybe<unsigned> {
  for(auto& item : state.items) {
    if(item->selected()) return (unsigned)item->offset();
  }
  return nothing;
}

auto mIconView::selectedItems() const -> vector<unsigned> {
  vector<unsigned> result;
  for(auto& item : state.items) {
    if(item->selected()) result.append(item->offset());
  }
  return result;
}

auto mIconView::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
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

auto mIconView::setMultiSelect(bool multiSelect) -> type& {
  state.multiSelect = multiSelect;
  signal(setMultiSelect, multiSelect);
  return *this;
}

auto mIconView::setOrientation(Orientation orientation) -> type& {
  state.orientation = orientation;
  signal(setOrientation, orientation);
  return *this;
}

auto mIconView::setSelected(const vector<signed>& selections) -> type& {
  bool selectAll = selections(0, 0) == ~0;
  for(auto& item : state.items) item->state.selected = selectAll;
  if(selectAll) return signal(setItemSelectedAll), *this;
  if(!selections) return signal(setItemSelectedNone), *this;
  for(auto& position : selections) {
    if(position >= items()) continue;
    state.items[position]->state.selected = true;
  }
  signal(setItemSelected, selections);
  return *this;
}
