#if defined(Hiro_Layout)

auto mLayout::allocate() -> pObject* {
  return new pLayout(*this);
}

auto mLayout::destruct() -> void {
  for(auto& sizable : state.sizables) sizable->destruct();
  mSizable::destruct();
}

//

auto mLayout::append(sSizable sizable) -> type& {
  state.sizables.append(sizable);
  sizable->setParent(this, sizableCount() - 1);
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

auto mLayout::remove(sSizable sizable) -> type& {
  auto offset = sizable->offset();
  sizable->setParent();
  state.sizables.remove(offset);
  for(auto n : range(offset, sizableCount())) {
    state.sizables[n]->adjustOffset(-1);
  }
  setGeometry(geometry());
  return *this;
}

auto mLayout::reset() -> type& {
  while(state.sizables) remove(state.sizables.right());
  return *this;
}

auto mLayout::setParent(mObject* parent, signed offset) -> type& {
  for(auto& sizable : state.sizables) sizable->destruct();
  mObject::setParent(parent, offset);
  for(auto& sizable : state.sizables) sizable->setParent(this, sizable->offset());
  return *this;
}

auto mLayout::sizable(unsigned position) const -> Sizable {
  if(position < sizableCount()) return state.sizables[position];
  return {};
}

auto mLayout::sizableCount() const -> unsigned {
  return state.sizables.size();
}

auto mLayout::sizables() const -> vector<Sizable> {
  vector<Sizable> sizables;
  for(auto& sizable : sizables) sizables.append(sizable);
  return sizables;
}

#endif
