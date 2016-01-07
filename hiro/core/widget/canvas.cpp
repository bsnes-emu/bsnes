#if defined(Hiro_Canvas)

auto mCanvas::allocate() -> pObject* {
  return new pCanvas(*this);
}

//

auto mCanvas::color() const -> Color {
  return state.color;
}

auto mCanvas::data() -> uint32* {
  return (uint32*)state.icon.data();
}

auto mCanvas::droppable() const -> bool {
  return state.droppable;
}

auto mCanvas::doDrop(lstring names) const -> void {
  if(state.onDrop) return state.onDrop(names);
}

auto mCanvas::doMouseLeave() const -> void {
  if(state.onMouseLeave) return state.onMouseLeave();
}

auto mCanvas::doMouseMove(Position position) const -> void {
  if(state.onMouseMove) return state.onMouseMove(position);
}

auto mCanvas::doMousePress(Mouse::Button button) const -> void {
  if(state.onMousePress) return state.onMousePress(button);
}

auto mCanvas::doMouseRelease(Mouse::Button button) const -> void {
  if(state.onMouseRelease) return state.onMouseRelease(button);
}

auto mCanvas::gradient() const -> Gradient {
  return state.gradient;
}

auto mCanvas::icon() const -> image {
  return state.icon;
}

auto mCanvas::onDrop(const function<void (lstring)>& callback) -> type& {
  state.onDrop = callback;
  return *this;
}

auto mCanvas::onMouseLeave(const function<void ()>& callback) -> type& {
  state.onMouseLeave = callback;
  return *this;
}

auto mCanvas::onMouseMove(const function<void (Position)>& callback) -> type& {
  state.onMouseMove = callback;
  return *this;
}

auto mCanvas::onMousePress(const function<void (Mouse::Button)>& callback) -> type& {
  state.onMousePress = callback;
  return *this;
}

auto mCanvas::onMouseRelease(const function<void (Mouse::Button)>& callback) -> type& {
  state.onMouseRelease = callback;
  return *this;
}

auto mCanvas::setColor(Color color) -> type& {
  state.color = color;
  signal(setColor, color);
  return *this;
}

auto mCanvas::setDroppable(bool droppable) -> type& {
  state.droppable = droppable;
  signal(setDroppable, droppable);
  return *this;
}

auto mCanvas::setGradient(Gradient gradient) -> type& {
  state.gradient = gradient;
  signal(setGradient, gradient);
  return *this;
}

auto mCanvas::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mCanvas::setSize(Size size) -> type& {
  image icon;
  icon.allocate(size.width(), size.height());
  return setIcon(icon);
}

auto mCanvas::size() const -> Size {
  return {(int)state.icon.width(), (int)state.icon.height()};
}

auto mCanvas::update() -> type& {
  signal(update);
  return *this;
}

#endif
