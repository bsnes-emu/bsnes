#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
}

auto pListViewItem::destruct() -> void {
}

auto pListViewItem::append(sListViewCell cell) -> void {
}

auto pListViewItem::remove(sListViewCell cell) -> void {
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
}

auto pListViewItem::setCheckable(bool checkable) -> void {
}

auto pListViewItem::setChecked(bool checked) -> void {
  if(auto parent = _parent()) {
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 0, checked, -1);
  }
}

auto pListViewItem::setFocused() -> void {
  if(auto parent = _parent()) {
    GtkTreePath* path = gtk_tree_path_new_from_string(string{self().offset()});
    gtk_tree_view_set_cursor(parent->gtkTreeView, path, nullptr, false);
    gtk_tree_view_scroll_to_cell(parent->gtkTreeView, path, nullptr, true, 0.5, 0.0);
    gtk_tree_path_free(path);
  }
}

auto pListViewItem::setForegroundColor(Color color) -> void {
}

auto pListViewItem::setSelected(bool selected) -> void {
  if(auto parent = _parent()) {
    parent->lock();
    if(selected) {
      gtk_tree_selection_select_iter(parent->gtkTreeSelection, &gtkIter);
    } else {
      gtk_tree_selection_unselect_iter(parent->gtkTreeSelection, &gtkIter);
    }
    parent->_updateSelected();
    parent->unlock();
  }
}

auto pListViewItem::_parent() -> pListView* {
  if(auto parent = self().parentListView()) return parent->self();
  return nullptr;
}

}

#endif
