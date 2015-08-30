#if defined(Hiro_ComboButton)

auto mComboButtonItem::allocate() -> pObject* {
  return new pComboButtonItem(*this);
}

//

auto mComboButtonItem::image() const -> Image {
  return state.image;
}

auto mComboButtonItem::remove() -> type& {
  if(auto comboButton = parentComboButton()) comboButton->remove(*this);
  return *this;
}

auto mComboButtonItem::selected() const -> bool {
  return state.selected;
}

auto mComboButtonItem::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mComboButtonItem::setSelected() -> type& {
  if(auto parent = parentComboButton()) {
    for(auto& item : parent->state.items) item->state.selected = false;
  }
  state.selected = true;
  signal(setSelected);
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
