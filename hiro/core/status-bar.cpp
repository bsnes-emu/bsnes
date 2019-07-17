#if defined(Hiro_StatusBar)

auto mStatusBar::allocate() -> pObject* {
  return new pStatusBar(*this);
}

//

auto mStatusBar::remove() -> type& {
  if(auto window = parentWindow()) window->remove(window->statusBar());
  return *this;
}

auto mStatusBar::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mStatusBar::text() const -> string {
  return state.text;
}

#endif
