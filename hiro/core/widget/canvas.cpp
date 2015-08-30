#if defined(Hiro_Canvas)

auto mCanvas::allocate() -> pObject* {
  return new pCanvas(*this);
}

//

auto mCanvas::color() const -> Color {
  return state.color;
}

auto mCanvas::data() -> uint32_t* {
  return state.image.data();
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

auto mCanvas::image() const -> Image {
  return state.image;
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

auto mCanvas::setImage(const Image& image) -> type& {
  state.image = image;
  signal(setImage, image);
  return *this;
}

auto mCanvas::setSize(Size size) -> type& {
  Image image;
  image.setSize(size);
  memory::fill(image.data(), size.width() * size.height() * sizeof(uint32_t), 0x00);
  return setImage(image);
}

auto mCanvas::size() const -> Size {
  return state.image.size();
}

auto mCanvas::update() -> type& {
  signal(update);
  return *this;
}

#endif
