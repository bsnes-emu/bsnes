#if defined(Hiro_Canvas)

auto mCanvas::allocate() -> pObject* {
  return new pCanvas(*this);
}

//

auto mCanvas::color() const -> Color {
  return state.color;
}

auto mCanvas::data() -> uint32_t* {
  return state.data.data();
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

auto mCanvas::gradient() const -> vector<Color> {
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
  state.size = {};
  state.color = color;
  signal(setColor, color);
  return *this;
}

auto mCanvas::setData(Size size) -> type& {
  state.size = size;
  state.data.resize(size.width() * size.height());
  memory::fill(state.data.data(), size.width() * size.height() * sizeof(uint32_t));
  signal(setData, size);
  return *this;
}

auto mCanvas::setDroppable(bool droppable) -> type& {
  state.droppable = droppable;
  signal(setDroppable, droppable);
  return *this;
}

auto mCanvas::setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> type& {
  state.size = {};
  state.gradient[0] = topLeft;
  state.gradient[1] = topRight;
  state.gradient[2] = bottomLeft;
  state.gradient[3] = bottomRight;
  signal(setGradient, topLeft, topRight, bottomLeft, bottomRight);
  return *this;
}

auto mCanvas::setHorizontalGradient(Color left, Color right) -> type& {
  return setGradient(left, right, left, right);
}

auto mCanvas::setIcon(const image& icon) -> type& {
  state.size = {(signed)icon.width, (signed)icon.height};
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mCanvas::setVerticalGradient(Color top, Color bottom) -> type& {
  return setGradient(top, top, bottom, bottom);
}

auto mCanvas::size() const -> Size {
  return state.size;
}

auto mCanvas::update() -> type& {
  signal(update);
  return *this;
}

#endif
