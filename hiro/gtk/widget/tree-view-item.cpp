#if defined(Hiro_TreeView)

namespace hiro {

auto pTreeViewItem::construct() -> void {
  if(auto parentWidget = _parentWidget()) {
    if(auto parentItem = _parentItem()) {
      gtk_tree_store_append(parentWidget->gtkTreeStore, &gtkIter, &parentItem->gtkIter);
    } else {
      gtk_tree_store_append(parentWidget->gtkTreeStore, &gtkIter, nullptr);
    }
    setChecked(state().checked);
    setIcon(state().icon);
    setText(state().text);
  }
}

auto pTreeViewItem::destruct() -> void {
  if(auto parent = _parentWidget()) {
    gtk_tree_store_remove(parent->gtkTreeStore, &gtkIter);
  }
}

//

auto pTreeViewItem::append(sTreeViewItem item) -> void {
}

auto pTreeViewItem::remove(sTreeViewItem item) -> void {
}

auto pTreeViewItem::setBackgroundColor(Color color) -> void {
}

auto pTreeViewItem::setCheckable(bool checkable) -> void {
}

auto pTreeViewItem::setChecked(bool checked) -> void {
  if(auto parentWidget = _parentWidget()) {
    gtk_tree_store_set(parentWidget->gtkTreeStore, &gtkIter, 0, checked, -1);
  }
}

auto pTreeViewItem::setExpanded(bool expanded) -> void {
  if(auto parentWidget = _parentWidget()) {
    auto path = gtk_tree_model_get_path(parentWidget->gtkTreeModel, &gtkIter);
    if(expanded) {
      gtk_tree_view_expand_row(parentWidget->gtkTreeView, path, false);
    } else {
      gtk_tree_view_collapse_row(parentWidget->gtkTreeView, path);
    }
    gtk_tree_path_free(path);
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

auto pTreeViewItem::setForegroundColor(Color color) -> void {
}

auto pTreeViewItem::setIcon(const image& icon) -> void {
  if(auto parentWidget = _parentWidget()) {
    if(icon) {
      auto pixbuf = CreatePixbuf(icon);
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

#endif
