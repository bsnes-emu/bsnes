#if defined(Hiro_Viewport)

auto mViewport::allocate() -> pObject* {
  return new pViewport(*this);
}

//

auto mViewport::doDrop(lstring names) const -> void {
  if(state.onDrop) return state.onDrop(names);
}

auto mViewport::doMouseLeave() const -> void {
  if(state.onMouseLeave) return state.onMouseLeave();
}

auto mViewport::doMouseMove(Position position) const -> void {
  if(state.onMouseMove) return state.onMouseMove(position);
}

auto mViewport::doMousePress(Mouse::Button button) const -> void {
  if(state.onMousePress) return state.onMousePress(button);
}

auto mViewport::doMouseRelease(Mouse::Button button) const -> void {
  if(state.onMouseRelease) return state.onMouseRelease(button);
}

auto mViewport::droppable() const -> bool {
  return state.droppable;
}

auto mViewport::handle() const -> uintptr_t {
  return signal(handle);
}

auto mViewport::onDrop(const function<void (lstring)>& function) -> type& {
  state.onDrop = function;
  return *this;
}

auto mViewport::onMouseLeave(const function<void ()>& function) -> type& {
  state.onMouseLeave = function;
  return *this;
}

auto mViewport::onMouseMove(const function<void (Position)>& function) -> type& {
  state.onMouseMove = function;
  return *this;
}

auto mViewport::onMousePress(const function<void (Mouse::Button)>& function) -> type& {
  state.onMousePress = function;
  return *this;
}

auto mViewport::onMouseRelease(const function<void (Mouse::Button)>& function) -> type& {
  state.onMouseRelease = function;
  return *this;
}

auto mViewport::setDroppable(bool droppable) -> type& {
  state.droppable = droppable;
  signal(setDroppable, droppable);
  return *this;
}

#endif
