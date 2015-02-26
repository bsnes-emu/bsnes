namespace hiro {

struct pTreeViewItem : pObject {
  Declare(TreeViewItem, Object)

  auto append(sTreeViewItem item) -> void;
  auto remove(sTreeViewItem item) -> void;
  auto setChecked(bool checked) -> void;
  auto setFocused() -> void;
  auto setIcon(const image& icon) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;

  auto _parentItem() -> pTreeViewItem*;
  auto _parentWidget() -> pTreeView*;

  GtkTreeIter gtkIter;
};

}
