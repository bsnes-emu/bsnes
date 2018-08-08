#if defined(Hiro_Widget)

auto mWidget::allocate() -> pObject* {
  return new pWidget(*this);
}

//

auto mWidget::remove() -> type& {
//TODO: how to implement this after removing mLayout?
//if(auto layout = parentLayout()) layout->remove(layout->sizable(offset()));
  setParent();
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
