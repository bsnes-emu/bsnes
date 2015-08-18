#if defined(Hiro_ListView)

namespace hiro {

auto pListViewItem::construct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    gtk_list_store_append(parent->gtkListStore, &gtkIter);
    _setState();
    parent->unlock();
  }
}

auto pListViewItem::destruct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    gtk_list_store_remove(parent->gtkListStore, &gtkIter);
    parent->_updateSelected();
    parent->unlock();
  }
}

auto pListViewItem::append(sListViewCell cell) -> void {
}

auto pListViewItem::remove(sListViewCell cell) -> void {
}

auto pListViewItem::setAlignment(Alignment alignment) -> void {
}

auto pListViewItem::setBackgroundColor(Color color) -> void {
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
  _setState();
}

auto pListViewItem::_parent() -> maybe<pListView&> {
  if(auto parent = self().parentListView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pListViewItem::_setState() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    if(state().selected) {
      gtk_tree_selection_select_iter(parent->gtkTreeSelection, &gtkIter);
    } else {
      gtk_tree_selection_unselect_iter(parent->gtkTreeSelection, &gtkIter);
    }
    parent->_updateSelected();
    for(auto& cell : state().cells) {
      if(auto self = cell->self()) self->_setState();
    }
    parent->unlock();
  }
}

}

#endif
