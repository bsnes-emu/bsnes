#if defined(Hiro_ComboButton)

auto mComboButton::allocate() -> pObject* {
  return new pComboButton(*this);
}

auto mComboButton::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mComboButton::append(sComboButtonItem item) -> type& {
  if(!state.items) item->state.selected = true;
  state.items.append(item);
  item->setParent(this, itemCount() - 1);
  signal(append, item);
  return *this;
}

auto mComboButton::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mComboButton::item(unsigned position) const -> ComboButtonItem {
  if(position < itemCount()) return state.items[position];
  return {};
}

auto mComboButton::itemCount() const -> unsigned {
  return state.items.size();
}

auto mComboButton::items() const -> vector<ComboButtonItem> {
  vector<ComboButtonItem> items;
  for(auto& item : state.items) items.append(item);
  return items;
}

auto mComboButton::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mComboButton::remove(sComboButtonItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), itemCount())) {
    state.items[n]->adjustOffset(-1);
  }
  item->setParent();
  return *this;
}

auto mComboButton::reset() -> type& {
  signal(reset);
  for(auto& item : state.items) item->setParent();
  state.items.reset();
  return *this;
}

auto mComboButton::selected() const -> ComboButtonItem {
  for(auto& item : state.items) {
    if(item->selected()) return item;
  }
  return {};
}

auto mComboButton::setParent(mObject* parent, signed offset) -> type& {
  for(auto& item : state.items) item->destruct();
  mObject::setParent(parent, offset);
  for(auto& item : state.items) item->setParent(this, item->offset());
  return *this;
}

#endif
