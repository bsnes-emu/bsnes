#if defined(Hiro_ListView)

mListViewItem::mListViewItem() {
  append(TableViewCell());
}

auto mListViewItem::checkable() const -> bool {
  return cell(0).checkable();
}

auto mListViewItem::checked() const -> bool {
  return cell(0).checked();
}

auto mListViewItem::icon() const -> image {
  return cell(0).icon();
}

auto mListViewItem::reset() -> type& {
  mTableViewItem::reset();
  append(TableViewCell());
  return *this;
}

auto mListViewItem::setCheckable(bool checkable) -> type& {
  cell(0).setCheckable(checkable);
  return *this;
}

auto mListViewItem::setChecked(bool checked) -> type& {
  cell(0).setChecked(checked);
  return *this;
}

auto mListViewItem::setIcon(const image& icon) -> type& {
  cell(0).setIcon(icon);
  return *this;
}

auto mListViewItem::setText(const string& text) -> type& {
  cell(0).setText(text);
  return *this;
}

auto mListViewItem::text() const -> string {
  return cell(0).text();
}

#endif
