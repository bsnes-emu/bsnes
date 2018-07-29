#if defined(Hiro_VerticalLayout)

auto mVerticalLayout::alignment() const -> maybe<float> {
  return state.alignment;
}

auto mVerticalLayout::append(sSizable sizable, Size size, float spacing) -> type& {
  VerticalLayoutCell cell;
  cell->setSizable(sizable);
  cell->setSize(size);
  cell->setSpacing(spacing);
  cell->setParent(this, cellCount());
  state.cells.append(cell);
  return synchronize();
}

auto mVerticalLayout::cell(uint position) const -> VerticalLayoutCell {
  return state.cells(position, {});
}

auto mVerticalLayout::cell(sSizable sizable) const -> VerticalLayoutCell {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return cell;
  }
  return {};
}

auto mVerticalLayout::cellCount() const -> uint {
  return state.cells.size();
}

auto mVerticalLayout::destruct() -> void {
  for(auto& cell : state.cells) cell->destruct();
  mSizable::destruct();
}

auto mVerticalLayout::minimumSize() const -> Size {
  float width = 0;
  for(auto index : range(cellCount())) {
    auto cell = this->cell(index);
    if(cell.size().width() == Size::Minimum || cell.size().width() == Size::Maximum) {
      width = max(width, cell.sizable().minimumSize().width());
      continue;
    }
    width = max(width, cell.size().width());
  }

  float height = 0;
  for(auto index : range(cellCount())) {
    auto cell = this->cell(index);
    if(cell.size().height() == Size::Minimum || cell.size().height() == Size::Maximum) {
      height += cell.sizable().minimumSize().height();
    } else {
      height += cell.size().height();
    }
    if(index != cellCount() - 1) height += cell.spacing();
  }

  return {
    padding().x() + width  + padding().width(),
    padding().y() + height + padding().height()
  };
}

auto mVerticalLayout::padding() const -> Geometry {
  return state.padding;
}

auto mVerticalLayout::remove(sSizable sizable) -> type& {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return remove(cell);
  }
  return *this;
}

auto mVerticalLayout::remove(sVerticalLayoutCell cell) -> type& {
  if(cell->parent() != this) return *this;
  auto offset = cell->offset();
  cell->setParent();
  state.cells.remove(offset);
  for(uint n : range(offset, cellCount())) state.cells[n]->adjustOffset(-1);
  return synchronize();
}

auto mVerticalLayout::reset() -> type& {
  while(state.cells) remove(state.cells.right());
  return synchronize();
}

auto mVerticalLayout::setAlignment(maybe<float> alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mVerticalLayout::setEnabled(bool enabled) -> type& {
  mSizable::setEnabled(enabled);
  for(auto& cell : state.cells) cell.sizable().setEnabled(cell.sizable().enabled());
  return *this;
}

auto mVerticalLayout::setFont(const Font& font) -> type& {
  mSizable::setFont(font);
  for(auto& cell : state.cells) cell.sizable().setFont(cell.sizable().font());
  return *this;
}

auto mVerticalLayout::setGeometry(Geometry geometry) -> type& {
  mSizable::setGeometry(geometry);

  geometry.setX(geometry.x() + padding().x());
  geometry.setY(geometry.y() + padding().y());
  geometry.setWidth (geometry.width()  - padding().x() - padding().width());
  geometry.setHeight(geometry.height() - padding().y() - padding().height());

  float width = 0;
  for(auto index : range(cellCount())) {
    auto cell = this->cell(index);
    if(cell.size().width() == Size::Maximum) {
      width = geometry.width();
      break;
    } else if(cell.size().width() == Size::Minimum) {
      width = max(width, cell.sizable().minimumSize().width());
    } else {
      width = max(width, cell.size().width());
    }
  }

  vector<float> heights;
  heights.resize(cellCount());
  uint maximumHeights = 0;
  for(auto index : range(cellCount())) {
    auto cell = this->cell(index);
    float height = 0;
    if(cell.size().height() == Size::Maximum) {
      height = Size::Maximum;
      maximumHeights++;
    } else if(cell.size().height() == Size::Minimum) {
      height = cell.sizable().minimumSize().height();
    } else {
      height = cell.size().height();
    }
    heights[index] = height;
  }

  float fixedHeight = 0;
  for(uint index : range(state.cells.size())) {
    if(heights[index] != Size::Maximum) fixedHeight += heights[index];
    if(index != cellCount() - 1) fixedHeight += cell(index).spacing();
  }

  float maximumHeight = (geometry.height() - fixedHeight) / maximumHeights;
  for(auto& height : heights) {
    if(height == Size::Maximum) height = maximumHeight;
  }

  float geometryX = geometry.x();
  float geometryY = geometry.y();
  for(auto index : range(cellCount())) {
    float geometryWidth  = width;
    float geometryHeight = heights[index];
    auto cell = this->cell(index);
    auto alignment = cell.alignment();
    if(!alignment) alignment = this->alignment();
    if(!alignment) alignment = 0.5;
    float cellWidth  = cell.size().width();
    float cellHeight = geometryHeight;
    if(cellWidth == Size::Minimum) cellWidth = cell.sizable()->minimumSize().width();
    if(cellWidth == Size::Maximum) cellWidth = geometryWidth;
    float cellX = geometryX + alignment() * (geometryWidth - cellWidth);
    float cellY = geometryY;
    cell.sizable().setGeometry({cellX, cellY, cellWidth, cellHeight});
    geometryY += geometryHeight + cell.spacing();
  }

  return *this;
}

auto mVerticalLayout::setPadding(Geometry padding) -> type& {
  state.padding = padding;
  return synchronize();
}

auto mVerticalLayout::setParent(mObject* parent, int offset) -> type& {
  for(auto& cell : reverse(state.cells)) cell->destruct();
  mSizable::setParent(parent, offset);
  for(auto& cell : state.cells) cell->setParent(this, cell->offset());
  return *this;
}

auto mVerticalLayout::setSpacing(float spacing) -> type& {
  state.spacing = spacing;
  return synchronize();
}

auto mVerticalLayout::setVisible(bool visible) -> type& {
  mSizable::setVisible(visible);
  for(auto& cell : state.cells) cell.sizable().setVisible(cell.sizable().visible());
  return synchronize();
}

auto mVerticalLayout::spacing() const -> float {
  return state.spacing;
}

auto mVerticalLayout::synchronize() -> type& {
  setGeometry(geometry());
  return *this;
}

//

auto mVerticalLayoutCell::alignment() const -> maybe<float> {
  return state.alignment;
}

auto mVerticalLayoutCell::destruct() -> void {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::destruct();
}

auto mVerticalLayoutCell::setAlignment(maybe<float> alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mVerticalLayoutCell::setEnabled(bool enabled) -> type& {
  mObject::setEnabled(enabled);
  state.sizable->setEnabled(state.sizable->enabled());
  return *this;
}

auto mVerticalLayoutCell::setFont(const Font& font) -> type& {
  mObject::setFont(font);
  state.sizable->setFont(state.sizable->font());
  return *this;
}

auto mVerticalLayoutCell::setParent(mObject* parent, int offset) -> type& {
  state.sizable->destruct();
  mObject::setParent(parent, offset);
  state.sizable->setParent(this, 0);
  return *this;
}

auto mVerticalLayoutCell::setSizable(sSizable sizable) -> type& {
  state.sizable = sizable;
  return synchronize();
}

auto mVerticalLayoutCell::setSize(Size size) -> type& {
  state.size = size;
  return synchronize();
}

auto mVerticalLayoutCell::setSpacing(float spacing) -> type& {
  state.spacing = spacing;
  return synchronize();
}

auto mVerticalLayoutCell::setVisible(bool visible) -> type& {
  mObject::setVisible(visible);
  state.sizable->setVisible(state.sizable->visible());
  return *this;
}

auto mVerticalLayoutCell::sizable() const -> Sizable {
  return state.sizable ? state.sizable : Sizable();
}

auto mVerticalLayoutCell::size() const -> Size {
  return state.size;
}

auto mVerticalLayoutCell::spacing() const -> float {
  return state.spacing;
}

auto mVerticalLayoutCell::synchronize() -> type& {
  if(auto parent = this->parent()) {
    if(auto verticalLayout = dynamic_cast<mVerticalLayout*>(parent)) {
      verticalLayout->synchronize();
    }
  }
  return *this;
}

#endif
