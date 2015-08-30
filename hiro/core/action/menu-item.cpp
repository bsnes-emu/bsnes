#if defined(Hiro_MenuItem)

auto mMenuItem::allocate() -> pObject* {
  return new pMenuItem(*this);
}

//

auto mMenuItem::doActivate() const -> void {
  if(state.onActivate) return state.onActivate();
}

auto mMenuItem::image() const -> Image {
  return state.image;
}

auto mMenuItem::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mMenuItem::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mMenuItem::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mMenuItem::text() const -> string {
  return state.text;
}

#endif
