#if defined(Hiro_Widget)

auto mWidget::allocate() -> pObject* {
  return new pWidget(*this);
}

//

auto mWidget::doSize() const -> void {
  if(state.onSize) return state.onSize();
}

auto mWidget::onSize(const function<void ()>& function) -> type& {
  state.onSize = function;
  return *this;
}

auto mWidget::remove() -> type& {
  if(auto layout = parentLayout()) layout->remove(layout->sizable(offset()));
  setParent();
  return *this;
}

#endif
