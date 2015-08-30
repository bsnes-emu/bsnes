#if defined(Hiro_IconView)

auto mIconViewItem::allocate() -> pObject* {
  return new pIconViewItem(*this);
}

//

auto mIconViewItem::image() const -> Image {
  return state.image;
}

auto mIconViewItem::remove() -> type& {
  if(auto iconView = parentIconView()) iconView->remove(*this);
  return *this;
}

auto mIconViewItem::selected() const -> bool {
  return state.selected;
}

auto mIconViewItem::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mIconViewItem::setSelected(bool selected) -> type& {
  state.selected = selected;
  signal(setSelected, selected);
  return *this;
}

auto mIconViewItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mIconViewItem::text() const -> string {
  return state.text;
}

#endif
