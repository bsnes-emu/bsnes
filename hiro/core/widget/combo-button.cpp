auto mComboButton::allocate() -> pObject* {
  return new pComboButton(*this);
}

auto mComboButton::destruct() -> void {
  for(auto& item : state.items) item->destruct();
  mWidget::destruct();
}

//

auto mComboButton::append(sComboButtonItem item) -> type& {
  state.items.append(item);
  item->setParent(this, items() - 1);
  signal(append, item);
  if(state.selected < 0) item->setSelected();
  return *this;
}

auto mComboButton::doChange() const -> void {
  if(state.onChange) return state.onChange();
}

auto mComboButton::item(unsigned position) const -> sComboButtonItem {
  if(position < items()) return state.items[position];
  return {};
}

auto mComboButton::items() const -> unsigned {
  return state.items.size();
}

auto mComboButton::onChange(const function<void ()>& function) -> type& {
  state.onChange = function;
  return *this;
}

auto mComboButton::remove(sComboButtonItem item) -> type& {
  signal(remove, item);
  state.items.remove(item->offset());
  for(auto n : range(item->offset(), items())) {
    state.items[n]->offset(-1);
  }
  item->setParent();
  return *this;
}

auto mComboButton::reset() -> type& {
  signal(reset);
  for(auto& item : state.items) {
    item->setParent();
  }
  state.items.reset();
  state.selected = -1;
  return *this;
}

auto mComboButton::selected() const -> sComboButtonItem {
  if(state.selected >= 0) return state.items[state.selected];
  return {};
}
