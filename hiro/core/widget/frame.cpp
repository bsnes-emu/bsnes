#if defined(Hiro_Frame)

auto mFrame::allocate() -> pObject* {
  return new pFrame(*this);
}

auto mFrame::destruct() -> void {
  if(auto& sizable = state.sizable) sizable->destruct();
  mWidget::destruct();
}

//

auto mFrame::append(sSizable sizable) -> type& {
  if(auto& sizable = state.sizable) remove(sizable);
  state.sizable = sizable;
  sizable->setParent(this, 0);
  signal(append, sizable);
  return *this;
}

auto mFrame::remove(sSizable sizable) -> type& {
  signal(remove, sizable);
  sizable->setParent();
  state.sizable.reset();
  return *this;
}

auto mFrame::reset() -> type& {
  if(auto& sizable = state.sizable) remove(sizable);
  return *this;
}

auto mFrame::setParent(mObject* object, int offset) -> type& {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::setParent(object, offset);
  if(auto& sizable = state.sizable) sizable->setParent(this, 0);
  return *this;
}

auto mFrame::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mFrame::sizable() const -> Sizable {
  return state.sizable;
}

auto mFrame::text() const -> string {
  return state.text;
}

#endif
