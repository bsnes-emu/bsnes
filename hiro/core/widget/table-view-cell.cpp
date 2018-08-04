#if defined(Hiro_TableView)

auto mTableViewCell::allocate() -> pObject* {
  return new pTableViewCell(*this);
}

//

auto mTableViewCell::alignment(bool recursive) const -> Alignment {
  if(auto alignment = state.alignment) return alignment;
  if(recursive) {
    if(auto parent = parentTableViewItem()) {
      if(auto alignment = parent->state.alignment) return alignment;
      if(auto grandparent = parent->parentTableView()) {
        if(offset() < grandparent->columnCount()) {
          if(auto column = grandparent->state.columns[offset()]) {
            if(auto alignment = column->state.alignment) return alignment;
          }
        }
        if(auto alignment = grandparent->state.alignment) return alignment;
      }
    }
  }
  return {};
}

auto mTableViewCell::backgroundColor(bool recursive) const -> Color {
  if(auto color = state.backgroundColor) return color;
  if(recursive) {
    if(auto parent = parentTableViewItem()) {
      if(auto color = parent->state.backgroundColor) return color;
      if(auto grandparent = parent->parentTableView()) {
        if(offset() < grandparent->columnCount()) {
          if(auto column = grandparent->state.columns[offset()]) {
            if(auto color = column->state.backgroundColor) return color;
          }
        }
        if(auto color = grandparent->state.backgroundColor) return color;
      }
    }
  }
  return {};
}

auto mTableViewCell::checkable() const -> bool {
  return state.checkable;
}

auto mTableViewCell::checked() const -> bool {
  return state.checkable && state.checked;
}

auto mTableViewCell::font(bool recursive) const -> Font {
  if(auto font = mObject::font()) return font;
  if(recursive) {
    if(auto parent = parentTableViewItem()) {
      if(auto font = parent->font()) return font;
      if(auto grandparent = parent->parentTableView()) {
        if(offset() < grandparent->columnCount()) {
          if(auto column = grandparent->state.columns[offset()]) {
            if(auto font = column->font()) return font;
          }
        }
        if(auto font = grandparent->font(true)) return font;
      }
    }
  }
  return {};
}

auto mTableViewCell::foregroundColor(bool recursive) const -> Color {
  if(auto color = state.foregroundColor) return color;
  if(recursive) {
    if(auto parent = parentTableViewItem()) {
      if(auto color = parent->state.foregroundColor) return color;
      if(auto grandparent = parent->parentTableView()) {
        if(offset() < grandparent->columnCount()) {
          if(auto column = grandparent->state.columns[offset()]) {
            if(auto color = column->state.foregroundColor) return color;
          }
        }
        if(auto color = grandparent->state.foregroundColor) return color;
      }
    }
  }
  return state.foregroundColor;
}

auto mTableViewCell::icon() const -> image {
  return state.icon;
}

auto mTableViewCell::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mTableViewCell::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mTableViewCell::setCheckable(bool checkable) -> type& {
  state.checkable = checkable;
  signal(setCheckable, checkable);
  return *this;
}

auto mTableViewCell::setChecked(bool checked) -> type& {
  setCheckable(true);
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mTableViewCell::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mTableViewCell::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mTableViewCell::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mTableViewCell::text() const -> string {
  return state.text;
}

#endif
