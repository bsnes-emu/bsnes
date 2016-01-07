#if defined(Hiro_ListView)

auto mListViewCell::allocate() -> pObject* {
  return new pListViewCell(*this);
}

//

auto mListViewCell::alignment(bool recursive) const -> Alignment {
  if(auto alignment = state.alignment) return alignment;
  if(recursive) {
    if(auto parent = parentListViewItem()) {
      if(auto alignment = parent->state.alignment) return alignment;
      if(auto grandparent = parent->parentListView()) {
        if(auto header = grandparent->state.header) {
          if(offset() < header->columnCount()) {
            if(auto column = header->state.columns[offset()]) {
              if(auto alignment = column->state.alignment) return alignment;
            }
          }
        }
        if(auto alignment = grandparent->state.alignment) return alignment;
      }
    }
  }
  return {};
}

auto mListViewCell::backgroundColor(bool recursive) const -> Color {
  if(auto color = state.backgroundColor) return color;
  if(recursive) {
    if(auto parent = parentListViewItem()) {
      if(auto color = parent->state.backgroundColor) return color;
      if(auto grandparent = parent->parentListView()) {
        if(auto header = grandparent->state.header) {
          if(offset() < header->columnCount()) {
            if(auto column = header->state.columns[offset()]) {
              if(auto color = column->state.backgroundColor) return color;
            }
          }
        }
        if(auto color = grandparent->state.backgroundColor) return color;
      }
    }
  }
  return {};
}

auto mListViewCell::checkable() const -> bool {
  return state.checkable;
}

auto mListViewCell::checked() const -> bool {
  return state.checkable && state.checked;
}

auto mListViewCell::font(bool recursive) const -> Font {
  if(auto font = mObject::font()) return font;
  if(recursive) {
    if(auto parent = parentListViewItem()) {
      if(auto font = parent->font()) return font;
      if(auto grandparent = parent->parentListView()) {
        if(auto header = grandparent->state.header) {
          if(offset() < header->columnCount()) {
            if(auto column = header->state.columns[offset()]) {
              if(auto font = column->font()) return font;
            }
          }
        }
        if(auto font = grandparent->font(true)) return font;
      }
    }
  }
  return {};
}

auto mListViewCell::foregroundColor(bool recursive) const -> Color {
  if(auto color = state.foregroundColor) return color;
  if(recursive) {
    if(auto parent = parentListViewItem()) {
      if(auto color = parent->state.foregroundColor) return color;
      if(auto grandparent = parent->parentListView()) {
        if(auto header = grandparent->state.header) {
          if(offset() < header->columnCount()) {
            if(auto column = header->state.columns[offset()]) {
              if(auto color = column->state.foregroundColor) return color;
            }
          }
        }
        if(auto color = grandparent->state.foregroundColor) return color;
      }
    }
  }
  return state.foregroundColor;
}

auto mListViewCell::icon() const -> image {
  return state.icon;
}

auto mListViewCell::setAlignment(Alignment alignment) -> type& {
  state.alignment = alignment;
  signal(setAlignment, alignment);
  return *this;
}

auto mListViewCell::setBackgroundColor(Color color) -> type& {
  state.backgroundColor = color;
  signal(setBackgroundColor, color);
  return *this;
}

auto mListViewCell::setCheckable(bool checkable) -> type& {
  state.checkable = checkable;
  signal(setCheckable, checkable);
  return *this;
}

auto mListViewCell::setChecked(bool checked) -> type& {
  setCheckable(true);
  state.checked = checked;
  signal(setChecked, checked);
  return *this;
}

auto mListViewCell::setForegroundColor(Color color) -> type& {
  state.foregroundColor = color;
  signal(setForegroundColor, color);
  return *this;
}

auto mListViewCell::setIcon(const image& icon) -> type& {
  state.icon = icon;
  signal(setIcon, icon);
  return *this;
}

auto mListViewCell::setText(const string& text) -> type& {
  state.text = text;
  signal(setText, text);
  return *this;
}

auto mListViewCell::text() const -> string {
  return state.text;
}

#endif
