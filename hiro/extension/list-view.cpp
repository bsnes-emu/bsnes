#if defined(Hiro_ListView)

mListView::mListView() {
  mTableView::onActivate([&] { doActivate(); });
  mTableView::onChange([&] { doChange(); });
  mTableView::onContext([&] { doContext(); });
  mTableView::onToggle([&](TableViewCell cell) {
    if(auto item = cell->parentTableViewItem()) {
      if(auto shared = item->instance.acquire()) {
        doToggle(ListViewItem{shared});
      }
    }
  });
  append(TableViewColumn().setExpandable());
}

auto mListView::batched() const -> vector<ListViewItem> {
  auto batched = mTableView::batched();
  vector<ListViewItem> result;
  for(auto item : batched) result.append(ListViewItem{item});
  return result;
}

auto mListView::doActivate() const -> void {
  if(state.onActivate) state.onActivate();
}

auto mListView::doChange() const -> void {
  if(state.onChange) state.onChange();
}

auto mListView::doContext() const -> void {
  if(state.onContext) state.onContext();
}

auto mListView::doToggle(ListViewItem item) const -> void {
  if(state.onToggle) state.onToggle(item);
}

auto mListView::item(uint position) const -> ListViewItem {
  return ListViewItem{mTableView::item(position)};
}

auto mListView::items() const -> vector<ListViewItem> {
  auto items = mTableView::items();
  vector<ListViewItem> result;
  for(auto item : items) result.append(ListViewItem{item});
  return result;
}

auto mListView::onActivate(const function<void ()>& callback) -> type& {
  state.onActivate = callback;
  return *this;
}

auto mListView::onChange(const function<void ()>& callback) -> type& {
  state.onChange = callback;
  return *this;
}

auto mListView::onContext(const function<void ()>& callback) -> type& {
  state.onContext = callback;
  return *this;
}

auto mListView::onToggle(const function<void (ListViewItem)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mListView::reset() -> type& {
  mTableView::reset();
  append(TableViewColumn().setExpandable());
  return *this;
}

auto mListView::resize() -> type& {
  mTableView::resizeColumns();
  return *this;
}

auto mListView::selected() const -> ListViewItem {
  return ListViewItem{mTableView::selected()};
}

auto mListView::setVisible(bool visible) -> type& {
  mTableView::setVisible(visible);
#if 0
  if(visible) {
    Application::processEvents();  //heavy-handed, but necessary for proper Widget geometry
    mTableView::resizeColumns();
  }
#endif
  return *this;
}

//

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
