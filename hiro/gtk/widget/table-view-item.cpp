#if defined(Hiro_TableView)

namespace hiro {

auto pTableViewItem::construct() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
    gtk_list_store_append(parent->gtkListStore, &gtkIter);
    _setState();
  }
}

auto pTableViewItem::destruct() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
    gtk_list_store_remove(parent->gtkListStore, &gtkIter);
    parent->_updateSelected();
  }
}

auto pTableViewItem::append(sTableViewCell cell) -> void {
}

auto pTableViewItem::remove(sTableViewCell cell) -> void {
}

auto pTableViewItem::setAlignment(Alignment alignment) -> void {
}

auto pTableViewItem::setBackgroundColor(Color color) -> void {
}

auto pTableViewItem::setFocused() -> void {
  if(auto parent = _parent()) {
    //calling setSelected() and then setFocused() right after sometimes fails to set focus
    Application::processEvents();

    auto lock = parent->acquire();
    GtkTreePath* path = gtk_tree_path_new_from_string(string{self().offset()});
    gtk_tree_view_set_cursor(parent->gtkTreeView, path, nullptr, false);
    gtk_tree_view_scroll_to_cell(parent->gtkTreeView, path, nullptr, true, 0.5, 0.0);
    gtk_tree_path_free(path);
  }
}

auto pTableViewItem::setForegroundColor(Color color) -> void {
}

auto pTableViewItem::setSelected(bool selected) -> void {
  _setState();
}

auto pTableViewItem::_parent() -> maybe<pTableView&> {
  if(auto parent = self().parentTableView()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pTableViewItem::_setState() -> void {
  if(auto parent = _parent()) {
    auto lock = parent->acquire();
    if(state().selected) {
      gtk_tree_selection_select_iter(parent->gtkTreeSelection, &gtkIter);
    } else {
      gtk_tree_selection_unselect_iter(parent->gtkTreeSelection, &gtkIter);
    }
    parent->_updateSelected();
    for(auto& cell : state().cells) {
      if(auto self = cell->self()) self->_setState();
    }
  }
}

}

#endif
