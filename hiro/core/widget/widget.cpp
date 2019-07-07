#if defined(Hiro_Widget)

auto mWidget::allocate() -> pObject* {
  return new pWidget(*this);
}

//

auto mWidget::doDrop(vector<string> names) const -> void {
  if(state.onDrop) return state.onDrop(names);
}

auto mWidget::doMouseEnter() const -> void {
  if(state.onMouseEnter) return state.onMouseEnter();
}

auto mWidget::doMouseLeave() const -> void {
  if(state.onMouseLeave) return state.onMouseLeave();
}

auto mWidget::doMouseMove(Position position) const -> void {
  if(state.onMouseMove) return state.onMouseMove(position);
}

auto mWidget::doMousePress(Mouse::Button button) const -> void {
  if(state.onMousePress) return state.onMousePress(button);
}

auto mWidget::doMouseRelease(Mouse::Button button) const -> void {
  if(state.onMouseRelease) return state.onMouseRelease(button);
}

auto mWidget::droppable() const -> bool {
  return state.droppable;
}

auto mWidget::focusable() const -> bool {
  return state.focusable;
}

auto mWidget::mouseCursor() const -> MouseCursor {
  return state.mouseCursor;
}

auto mWidget::onDrop(const function<void (vector<string>)>& callback) -> type& {
  state.onDrop = callback;
  return *this;
}

auto mWidget::onMouseEnter(const function<void ()>& callback) -> type& {
  state.onMouseEnter = callback;
  return *this;
}

auto mWidget::onMouseLeave(const function<void ()>& callback) -> type& {
  state.onMouseLeave = callback;
  return *this;
}

auto mWidget::onMouseMove(const function<void (Position)>& callback) -> type& {
  state.onMouseMove = callback;
  return *this;
}

auto mWidget::onMousePress(const function<void (Mouse::Button)>& callback) -> type& {
  state.onMousePress = callback;
  return *this;
}

auto mWidget::onMouseRelease(const function<void (Mouse::Button)>& callback) -> type& {
  state.onMouseRelease = callback;
  return *this;
}

auto mWidget::remove() -> type& {
//TODO: how to implement this after removing mLayout?
//if(auto layout = parentLayout()) layout->remove(layout->sizable(offset()));
  setParent();
  return *this;
}

auto mWidget::setDroppable(bool droppable) -> type& {
  state.droppable = droppable;
  signal(setDroppable, droppable);
  return *this;
}

auto mWidget::setFocusable(bool focusable) -> type& {
  state.focusable = focusable;
  signal(setFocusable, focusable);
  return *this;
}

auto mWidget::setMouseCursor(const MouseCursor& mouseCursor) -> type& {
  state.mouseCursor = mouseCursor;
  signal(setMouseCursor, mouseCursor);
  return *this;
}

auto mWidget::setToolTip(const string& toolTip) -> type& {
  state.toolTip = toolTip;
  //TODO: allow this option to dynamically control tool tips
  if(!Application::state().toolTips) return *this;
  signal(setToolTip, toolTip);
  return *this;
}

auto mWidget::toolTip() const -> string {
  return state.toolTip;
}

#endif
