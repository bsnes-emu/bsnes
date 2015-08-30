#if defined(Hiro_TreeView)

namespace hiro {

struct pTreeViewItem : pObject {
  Declare(TreeViewItem, Object)

  auto append(sTreeViewItem item) -> void;
  auto remove(sTreeViewItem item) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setCheckable(bool checkable) -> void;
  auto setChecked(bool checked) -> void;
  auto setExpanded(bool expanded) -> void;
  auto setFocused() -> void;
  auto setForegroundColor(Color color) -> void;
  auto setImage(const Image& image) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;

  auto _parentItem() -> pTreeViewItem*;
  auto _parentWidget() -> pTreeView*;

  GtkTreeIter gtkIter;
};

}

#endif
