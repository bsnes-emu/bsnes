#if defined(Hiro_Layout)

auto mLayout::allocate() -> pObject* {
  return new pLayout(*this);
}

auto mLayout::destruct() -> void {
  for(auto& sizable : state.sizables) sizable->destruct();
  mSizable::destruct();
}

//

auto mLayout::append(shared_pointer<mSizable> sizable) -> type& {
  state.sizables.append(sizable);
  sizable->setParent(this, sizables() - 1);
  setGeometry(geometry());
  return *this;
}

auto mLayout::remove() -> type& {
  #if defined(Hiro_TabFrame)
  if(auto tabFrameItem = parentTabFrameItem()) tabFrameItem->remove(tabFrameItem->layout());
  #endif
  if(auto layout = parentLayout()) layout->remove(layout->sizable(offset()));
  if(auto window = parentWindow()) window->remove(window->layout());
  setParent();
  for(auto& sizable : state.sizables) sizable->setParent(this, sizable->offset());
  return *this;
}

auto mLayout::remove(shared_pointer<mSizable> sizable) -> type& {
  auto offset = sizable->offset();
  sizable->setParent();
  state.sizables.remove(offset);
  for(auto n : range(offset, sizables())) {
    state.sizables[n]->offset(-1);
  }
  setGeometry(geometry());
  return *this;
}

auto mLayout::reset() -> type& {
  while(state.sizables) remove(state.sizables.last());
  return *this;
}

auto mLayout::setParent(mObject* parent, signed offset) -> type& {
  for(auto& sizable : state.sizables) sizable->destruct();
  mObject::setParent(parent, offset);
  for(auto& sizable : state.sizables) sizable->setParent(this, sizable->offset());
  return *this;
}

auto mLayout::sizable(unsigned position) const -> shared_pointer<mSizable> {
  if(position >= sizables()) throw;
  return state.sizables[position];
}

auto mLayout::sizables() const -> unsigned {
  return state.sizables.size();
}

#endif
