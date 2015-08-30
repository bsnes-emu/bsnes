#if defined(Hiro_CheckButton)

auto mCheckButton::allocate() -> pObject* {
  return new pCheckButton(*this);
}

//

auto mCheckButton::bordered() const -> bool {
  return state.bordered;
}

auto mCheckButton::checked() const -> bool {
  return state.checked;
}

auto mCheckButton::doToggle() const -> void {
  if(state.onToggle) return state.onToggle();
}

auto mCheckButton::image() const -> Image {
  return state.image;
}

auto mCheckButton::onToggle(const function<void ()>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mCheckButton::orientation() const -> Orientation {
  return state.orientation;
}

auto mCheckButton::setBordered(bool bordered) -> type& {
  state.bordered = bordered;
  signal(setBordered, bordered);
  return *this;
}

auto mCheckButton::setChecked(bool checked) -> type& {
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mCheckButton::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mCheckButton::setOrientation(Orientation orientation) -> type& {
  state.orientation = orientation;
  signal(setOrientation, orientation);
  return *this;
}

auto mCheckButton::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mCheckButton::text() const -> string {
  return state.text;
}

#endif
