#if defined(Hiro_TableLayout)

auto mTableLayout::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableLayout::append(sSizable sizable, Size size) -> type& {
  TableLayoutCell cell;
  cell->setSizable(sizable);
  cell->setSize(size);
  cell->setParent(this, cellCount());
  state.cells.append(cell);
  return *this;
}

auto mTableLayout::cell(uint position) const -> TableLayoutCell {
  return state.cells(position, {});
}

auto mTableLayout::cell(uint x, uint y) const -> TableLayoutCell {
  if(auto cell = state.cells(y * columnCount() + x, {})) return cell;
  return {};
}

auto mTableLayout::cell(sSizable sizable) const -> TableLayoutCell {
  for(auto& cell : state.cells) {
    if(cell->state.sizable == sizable) return cell;
  }
  return {};
}

auto mTableLayout::cellCount() const -> uint {
  return state.cells.size();
}

auto mTableLayout::column(uint position) const -> TableLayoutColumn {
  return state.columns(position, {});
}

auto mTableLayout::columnCount() const -> uint {
  return state.columns.size();
}

auto mTableLayout::destruct() -> void {
  for(auto& cell : state.cells) cell->destruct();
  for(auto& column : state.columns) column->destruct();
  for(auto& row : state.rows) row->destruct();
  mSizable::destruct();
}

auto mTableLayout::minimumSize() const -> Size {
  float minimumWidth = 0;
  for(uint x : range(columnCount())) {
    float width = 0;
    auto column = this->column(x);
    for(uint y : range(rowCount())) {
      auto row = this->row(y);
      auto cell = this->cell(x, y);
      if(cell.size().width() == Size::Minimum || cell.size().width() == Size::Maximum) {
        width = max(width, cell.sizable()->minimumSize().width());
      } else {
        width = max(width, cell.size().width());
      }
    }
    minimumWidth += width;
    if(x != columnCount() - 1) minimumWidth += column.spacing();
  }

  float minimumHeight = 0;
  for(uint y : range(rowCount())) {
    float height = 0;
    auto row = this->row(y);
    for(uint x : range(columnCount())) {
      auto column = this->column(x);
      auto cell = this->cell(x, y);
      if(cell.size().height() == Size::Minimum || cell.size().height() == Size::Maximum) {
        height = max(height, cell.sizable()->minimumSize().height());
      } else {
        height = max(height, cell.size().height());
      }
    }
    minimumHeight += height;
    if(y != rowCount() - 1) minimumHeight += row.spacing();
  }

  return {
    padding().x() + minimumWidth  + padding().width(),
    padding().y() + minimumHeight + padding().height()
  };
}

auto mTableLayout::padding() const -> Geometry {
  return state.padding;
}

auto mTableLayout::remove(sTableLayoutCell cell) -> type& {
  if(cell->parent() != this) return *this;
  auto offset = cell->offset();
  cell->setParent();
  state.cells.remove(offset);
  for(uint n : range(offset, cellCount())) state.cells[n]->adjustOffset(-1);
  return synchronize();
}

auto mTableLayout::reset() -> type& {
  while(state.cells) remove(state.cells.right());
  return synchronize();
}

auto mTableLayout::row(uint position) const -> TableLayoutRow {
  return state.rows(position, {});
}

auto mTableLayout::rowCount() const -> uint {
  return state.rows.size();
}

auto mTableLayout::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mTableLayout::setEnabled(bool enabled) -> type& {
  mSizable::setEnabled(enabled);
  for(auto& cell : state.cells) cell.setEnabled(cell.enabled());
  return *this;
}

auto mTableLayout::setFont(const Font& font) -> type& {
  mSizable::setFont(font);
  for(auto& cell : state.cells) cell.setFont(cell.font());
  return *this;
}

auto mTableLayout::setGeometry(Geometry geometry) -> type& {
  mSizable::setGeometry(geometry);

  geometry.setX(geometry.x() + padding().x());
  geometry.setY(geometry.y() + padding().y());
  geometry.setWidth (geometry.width()  - padding().x() - padding().width());
  geometry.setHeight(geometry.height() - padding().y() - padding().height());

  vector<float> widths;
  widths.resize(columnCount());
  uint maximumWidths = 0;
  for(uint x : range(columnCount())) {
    float width = 0;
    auto column = this->column(x);
    for(uint y : range(rowCount())) {
      auto row = this->row(y);
      auto cell = this->cell(x, y);
      if(cell.size().width() == Size::Maximum) {
        width = Size::Maximum;
        maximumWidths++;
        break;
      }
      if(cell.size().width() == Size::Minimum) {
        width = max(width, cell.sizable()->minimumSize().width());
      } else {
        width = max(width, cell.size().width());
      }
    }
    widths[x] = width;
  }

  vector<float> heights;
  heights.resize(rowCount());
  uint maximumHeights = 0;
  for(uint y : range(rowCount())) {
    float height = 0;
    auto row = this->row(y);
    for(uint x : range(columnCount())) {
      auto column = this->column(x);
      auto cell = this->cell(x, y);
      if(cell.size().height() == Size::Maximum) {
        height = Size::Maximum;
        maximumHeights++;
        break;
      }
      if(cell.size().height() == Size::Minimum) {
        height = max(height, cell.sizable()->minimumSize().height());
      } else {
        height = max(height, cell.size().height());
      }
    }
    heights[y] = height;
  }

  float fixedWidth = 0;
  for(uint x : range(columnCount())) {
    if(widths[x] != Size::Maximum) fixedWidth += widths[x];
    if(x != columnCount() - 1) fixedWidth += column(x)->spacing();
  }
  float maximumWidth = (geometry.width() - fixedWidth) / maximumWidths;
  for(auto& width : widths) {
    if(width == Size::Maximum) width = maximumWidth;
  }

  float fixedHeight = 0;
  for(uint y : range(rowCount())) {
    if(heights[y] != Size::Maximum) fixedHeight += heights[y];
    if(y != rowCount() - 1) fixedHeight += row(y)->spacing();
  }
  float maximumHeight = (geometry.height() - fixedHeight) / maximumHeights;
  for(auto& height : heights) {
    if(height == Size::Maximum) height = maximumHeight;
  }

  float geometryY = geometry.y();
  for(uint y : range(rowCount())) {
    float geometryX = geometry.x();
    auto row = this->row(y);
    for(uint x : range(columnCount())) {
      auto column = this->column(x);
      auto cell = this->cell(x, y);
      float geometryWidth  = widths [x];
      float geometryHeight = heights[y];
      auto alignment = cell.alignment();
      if(!alignment) alignment = column.alignment();
      if(!alignment) alignment = row.alignment();
      if(!alignment) alignment = this->alignment();
      if(!alignment) alignment = {0.0, 0.5};

      float cellWidth = cell.size().width();
      if(cellWidth == Size::Minimum) cellWidth = cell.sizable()->minimumSize().width();
      if(cellWidth == Size::Maximum) cellWidth = geometryWidth;
      cellWidth = min(cellWidth, geometryWidth);
      float cellHeight = cell.size().height();
      if(cellHeight == Size::Minimum) cellHeight = cell.sizable()->minimumSize().height();
      if(cellHeight == Size::Maximum) cellHeight = geometryHeight;
      cellHeight = min(cellHeight, geometryHeight);
      float cellX = geometryX + alignment.horizontal() * (geometryWidth  - cellWidth);
      float cellY = geometryY + alignment.vertical()   * (geometryHeight - cellHeight);
      cell.sizable()->setGeometry({cellX, cellY, cellWidth, cellHeight});

      geometryX += widths[x] + column.spacing();
    }
    geometryY += heights[y] + row.spacing();
  }

  return *this;
}

auto mTableLayout::setPadding(Geometry padding) -> type& {
  state.padding = padding;
  return synchronize();
}

auto mTableLayout::setParent(mObject* parent, int offset) -> type& {
  for(auto& cell : reverse(state.cells)) cell->destruct();
  for(auto& column : reverse(state.columns)) column->destruct();
  for(auto& row : reverse(state.rows)) row->destruct();
  mObject::setParent(parent, offset);
  for(auto& cell : state.cells) cell->setParent(this, cell->offset());
  for(auto& column : state.columns) column->setParent(this, column->offset());
  for(auto& row : state.rows) row->setParent(this, row->offset());
  return *this;
}

auto mTableLayout::setSize(Size size) -> type& {
  state.size = size;
  state.columns.reset();
  state.rows.reset();
  for(auto x : range(size.width())) state.columns.append(TableLayoutColumn());
  for(auto y : range(size.height())) state.rows.append(TableLayoutRow());
  return synchronize();
}

auto mTableLayout::setVisible(bool visible) -> type& {
  mSizable::setVisible(visible);
  for(auto& cell : state.cells) cell.setVisible(cell.visible());
  return synchronize();
}

auto mTableLayout::size() const -> Size {
  return state.size;
}

auto mTableLayout::synchronize() -> type& {
  setGeometry(geometry());
  return *this;
}

//

auto mTableLayoutColumn::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableLayoutColumn::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mTableLayoutColumn::setSpacing(float spacing) -> type& {
  state.spacing = spacing;
  return synchronize();
}

auto mTableLayoutColumn::spacing() const -> float {
  return state.spacing;
}

auto mTableLayoutColumn::synchronize() -> type& {
  if(auto parent = this->parent()) {
    if(auto tableLayout = dynamic_cast<mTableLayout*>(parent)) {
      tableLayout->synchronize();
    }
  }
  return *this;
}

//

auto mTableLayoutRow::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableLayoutRow::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mTableLayoutRow::setSpacing(float spacing) -> type& {
  state.spacing = spacing;
  return synchronize();
}

auto mTableLayoutRow::spacing() const -> float {
  return state.spacing;
}

auto mTableLayoutRow::synchronize() -> type& {
  if(auto parent = this->parent()) {
    if(auto tableLayout = dynamic_cast<mTableLayout*>(parent)) {
      tableLayout->synchronize();
    }
  }
  return *this;
}

//

auto mTableLayoutCell::alignment() const -> Alignment {
  return state.alignment;
}

auto mTableLayoutCell::destruct() -> void {
  if(auto& sizable = state.sizable) sizable->destruct();
  mObject::destruct();
}

auto mTableLayoutCell::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  return synchronize();
}

auto mTableLayoutCell::setEnabled(bool enabled) -> type& {
  mObject::setEnabled(enabled);
  state.sizable->setEnabled(state.sizable->enabled());
  return *this;
}

auto mTableLayoutCell::setFont(const Font& font) -> type& {
  mObject::setFont(font);
  state.sizable->setFont(state.sizable->font());
  return *this;
}

auto mTableLayoutCell::setParent(mObject* parent, int offset) -> type& {
  state.sizable->destruct();
  mObject::setParent(parent, offset);
  state.sizable->setParent(this, 0);
  return *this;
}

auto mTableLayoutCell::setSizable(sSizable sizable) -> type& {
  state.sizable = sizable;
  state.sizable->setParent(this, 0);
  return synchronize();
}

auto mTableLayoutCell::setSize(Size size) -> type& {
  state.size = size;
  return synchronize();
}

auto mTableLayoutCell::setVisible(bool visible) -> type& {
  mObject::setVisible(visible);
  state.sizable->setVisible(state.sizable->visible());
  return *this;
}

auto mTableLayoutCell::sizable() const -> Sizable {
  return state.sizable ? state.sizable : Sizable();
}

auto mTableLayoutCell::size() const -> Size {
  return state.size;
}

auto mTableLayoutCell::synchronize() -> type& {
  if(auto parent = this->parent()) {
    if(auto tableLayout = dynamic_cast<mTableLayout*>(parent)) {
      tableLayout->synchronize();
    }
  }
  return *this;
}

#endif
