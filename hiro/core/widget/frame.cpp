#if defined(Hiro_Frame)

auto mFrame::allocate() -> pObject* {
  return new pFrame(*this);
}

auto mFrame::destruct() -> void {
  if(auto& layout = state.layout) layout->destruct();
  mWidget::destruct();
}

//

auto mFrame::append(sLayout layout) -> type& {
  if(auto& layout = state.layout) remove(layout);
  state.layout = layout;
  layout->setParent(this, 0);
  signal(append, layout);
  return *this;
}

auto mFrame::layout() const -> Layout {
  return state.layout;
}

auto mFrame::remove(sLayout layout) -> type& {
  signal(remove, layout);
  layout->setParent();
  state.layout.reset();
  return *this;
}

auto mFrame::reset() -> type& {
  if(auto& layout = state.layout) remove(layout);
  return *this;
}

auto mFrame::setParent(mObject* object, signed offset) -> type& {
  if(auto& layout = state.layout) layout->destruct();
  mObject::setParent(object, offset);
  if(auto& layout = state.layout) layout->setParent(this, 0);
  return *this;
}

auto mFrame::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mFrame::text() const -> string {
  return state.text;
}

#endif
