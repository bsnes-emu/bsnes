namespace hiro {

auto pTreeViewItem::construct() -> void {
}

auto pTreeViewItem::destruct() -> void {
}

//

auto pTreeViewItem::append(sTreeViewItem item) -> void {
  if(auto parentWidget = _parentWidget()) {
    gtk_tree_store_append(parentWidget->gtkTreeStore, &item->self()->gtkIter, &gtkIter);
    item->setChecked(item->checked());
    item->setIcon(item->icon());
    item->setText(item->text());
  }
}

auto pTreeViewItem::remove(sTreeViewItem item) -> void {
  if(auto parentWidget = _parentWidget()) {
    gtk_tree_store_remove(parentWidget->gtkTreeStore, &item->self()->gtkIter);
  }
}

auto pTreeViewItem::setChecked(bool checked) -> void {
  if(auto parentWidget = _parentWidget()) {
    gtk_tree_store_set(parentWidget->gtkTreeStore, &gtkIter, 0, checked, -1);
  }
}

auto pTreeViewItem::setFocused() -> void {
  if(auto parentWidget = _parentWidget()) {
    GtkTreePath* path = gtk_tree_path_new_from_string(self().path().transform("/", ":"));
    gtk_tree_view_set_cursor(parentWidget->gtkTreeView, path, nullptr, false);
    gtk_tree_view_scroll_to_cell(parentWidget->gtkTreeView, path, nullptr, true, 0.5, 0.0);
    gtk_tree_path_free(path);
  }
}

auto pTreeViewItem::setIcon(const image& icon) -> void {
  if(auto parentWidget = _parentWidget()) {
    if(icon) {
      auto pixbuf = CreatePixbuf(icon, true);
      gtk_tree_store_set(parentWidget->gtkTreeStore, &gtkIter, 1, pixbuf, -1);
    } else {
      gtk_tree_store_set(parentWidget->gtkTreeStore, &gtkIter, 1, nullptr, -1);
    }
  }
}

auto pTreeViewItem::setSelected() -> void {
  if(auto parentWidget = _parentWidget()) {
    parentWidget->lock();
    //in order to select an item, it must first be visible
    auto gtkPath = gtk_tree_model_get_path(parentWidget->gtkTreeModel, &gtkIter);
    gtk_tree_view_expand_to_path(parentWidget->gtkTreeView, gtkPath);
    gtk_tree_path_free(gtkPath);
    gtk_tree_selection_select_iter(parentWidget->gtkTreeSelection, &gtkIter);
    parentWidget->unlock();
  }
}

auto pTreeViewItem::setText(const string& text) -> void {
  if(auto parentWidget = _parentWidget()) {
    gtk_tree_store_set(parentWidget->gtkTreeStore, &gtkIter, 2, (const char*)text, -1);
  }
}

//

auto pTreeViewItem::_parentItem() -> pTreeViewItem* {
  if(auto parentItem = self().parentTreeViewItem()) return parentItem->self();
  return nullptr;
}

auto pTreeViewItem::_parentWidget() -> pTreeView* {
  if(auto parentWidget = self().parentTreeView(true)) return parentWidget->self();
  return nullptr;
}

}
