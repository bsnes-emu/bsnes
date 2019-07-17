#if defined(Hiro_FixedLayout)

auto mFixedLayout::append(sSizable sizable, Geometry geometry) -> type& {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return *this;
  }

  FixedLayoutCell cell;
  cell->setSizable(sizable);
  cell->setGeometry(geometry);
  cell->setParent(this, cellCount());
  state.cells.append(cell);
  return synchronize();
}

auto mFixedLayout::cell(uint position) const -> FixedLayoutCell {
  return state.cells(position, {});
}

auto mFixedLayout::cell(sSizable sizable) const -> FixedLayoutCell {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return cell;
  }
  return {};
}

auto mFixedLayout::cells() const -> vector<FixedLayoutCell> {
  return state.cells;
}

auto mFixedLayout::cellCount() const -> uint {
  return state.cells.size();
}

auto mFixedLayout::destruct() -> void {
  for(auto& cell : state.cells) cell->destruct();
  return mSizable::destruct();
}

auto mFixedLayout::minimumSize() const -> Size {
  float width = 0, height = 0;
  for(auto& cell : state.cells) {
    width  = max(width,  cell.sizable().minimumSize().width());
    height = max(height, cell.sizable().minimumSize().height());
  }
  return {width, height};
}

auto mFixedLayout::remove(sSizable sizable) -> type& {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return remove(cell);
  }
  return *this;
}

auto mFixedLayout::remove(sFixedLayoutCell cell) -> type& {
  if(cell->parent() != this) return *this;
  auto offset = cell->offset();
  cell->setParent();
  state.cells.remove(offset);
  for(uint n : range(offset, cellCount())) state.cells[n]->adjustOffset(-1);
  return synchronize();
}

auto mFixedLayout::reset() -> type& {
  while(state.cells) remove(state.cells.right());
  return synchronize();
}

auto mFixedLayout::resize() -> type& {
  setGeometry(geometry());
  return *this;
}

auto mFixedLayout::setEnabled(bool enabled) -> type& {
  mSizable::setEnabled(enabled);
  for(auto& cell : state.cells) cell.sizable().setEnabled(cell.sizable().enabled());
  return *this;
}

auto mFixedLayout::setFont(const Font& font) -> type& {
  mSizable::setFont(font);
  for(auto& cell : state.cells) cell.sizable().setFont(cell.sizable().font());
  return *this;
}

auto mFixedLayout::setParent(mObject* parent, int offset) -> type& {
  for(auto& cell : reverse(state.cells)) cell->destruct();
  mSizable::setParent(parent, offset);
  for(auto& cell : state.cells) cell->setParent(this, cell->offset());
  return *this;
}

auto mFixedLayout::setVisible(bool visible) -> type& {
  mSizable::setVisible(visible);
  for(auto& cell : state.cells) cell.sizable().setVisible(cell.sizable().visible());
  return synchronize();
}

auto mFixedLayout::synchronize() -> type& {
  setGeometry(geometry());
  return *this;
}

//

auto mFixedLayoutCell::destruct() -> void {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::destruct();
}

auto mFixedLayoutCell::geometry() const -> Geometry {
  return state.geometry;
}

auto mFixedLayoutCell::setEnabled(bool enabled) -> type& {
  mObject::setEnabled(enabled);
  state.sizable->setEnabled(state.sizable->enabled());
  return *this;
}

auto mFixedLayoutCell::setFont(const Font& font) -> type& {
  mObject::setFont(font);
  state.sizable->setFont(state.sizable->font());
  return *this;
}

auto mFixedLayoutCell::setGeometry(Geometry geometry) -> type& {
  state.geometry = geometry;
  return synchronize();
}

auto mFixedLayoutCell::setParent(mObject* parent, int offset) -> type& {
  state.sizable->destruct();
  mObject::setParent(parent, offset);
  state.sizable->setParent(this, 0);
  return *this;
}

auto mFixedLayoutCell::setSizable(sSizable sizable) -> type& {
  state.sizable = sizable;
  return synchronize();
}

auto mFixedLayoutCell::setVisible(bool visible) -> type& {
  mObject::setVisible(visible);
  state.sizable->setVisible(state.sizable->visible());
  return *this;
}

auto mFixedLayoutCell::sizable() const -> Sizable {
  return state.sizable ? state.sizable : Sizable();
}

auto mFixedLayoutCell::synchronize() -> type& {
  if(auto parent = this->parent()) {
    if(auto fixedLayout = dynamic_cast<mFixedLayout*>(parent)) {
      fixedLayout->synchronize();
    }
  }
  return *this;
}

#endif
