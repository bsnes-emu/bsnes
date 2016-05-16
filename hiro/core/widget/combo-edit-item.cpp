#if defined(Hiro_ComboEdit)

auto mComboEditItem::allocate() -> pObject* {
  return new pComboEditItem(*this);
}

//

auto mComboEditItem::icon() const -> image {
  return state.icon;
}

auto mComboEditItem::remove() -> type& {
  if(auto comboEdit = parentComboEdit()) comboEdit->remove(*this);
  return *this;
}

auto mComboEditItem::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mComboEditItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mComboEditItem::text() const -> string {
  return state.text;
}

#endif
