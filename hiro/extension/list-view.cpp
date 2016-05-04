#if defined(Hiro_ListView)

mListView::mListView() {
  mTableView::onToggle([&](TableViewCell cell) {
    if(auto item = cell->parentTableViewItem()) {
      if(auto shared = item->instance.acquire()) {
        doToggle(ListViewItem{shared});
      }
    }
  });
  append(TableViewHeader().setVisible(false).append(TableViewColumn().setExpandable()));
}

auto mListView::batched() const -> vector<ListViewItem> {
  auto batched = mTableView::batched();
  vector<ListViewItem> result;
  for(auto item : batched) result.append(ListViewItem{item});
  return result;
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

auto mListView::onToggle(const function<void (ListViewItem)>& callback) -> type& {
  state.onToggle = callback;
  return *this;
}

auto mListView::reset() -> type& {
  mTableView::reset();
  append(TableViewHeader().setVisible(false).append(TableViewColumn().setExpandable()));
  return *this;
}

auto mListView::selected() const -> ListViewItem {
  return ListViewItem{mTableView::selected()};
}

#endif
