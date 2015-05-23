#if defined(Hiro_ComboButton)

auto mComboButtonItem::allocate() -> pObject* {
  return new pComboButtonItem(*this);
}

//

auto mComboButtonItem::icon() const -> image {
  return state.icon;
}

auto mComboButtonItem::remove() -> type& {
  if(auto comboButton = parentComboButton()) comboButton->remove(*this);
  return *this;
}

auto mComboButtonItem::selected() const -> bool {
  if(auto comboButton = parentComboButton()) return comboButton->state.selected == offset();
  return false;
}

auto mComboButtonItem::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mComboButtonItem::setSelected() -> type& {
  if(auto parent = parentComboButton()) {
    parent->state.selected = offset();
    signal(setSelected);
  }
  return *this;
}

auto mComboButtonItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mComboButtonItem::text() const -> string {
  return state.text;
}

#endif
