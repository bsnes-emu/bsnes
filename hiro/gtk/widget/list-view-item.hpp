namespace hiro {

struct pListViewItem : pObject {
  Declare(ListViewItem, Object)

  auto setChecked(bool checked) -> void;
  auto setFocused() -> void;
  auto setIcon(unsigned column, const image& icon) -> void;
  auto setSelected(bool selected) -> void;
  auto setText(unsigned column, const string& text) -> void;

  auto _parent() -> pListView*;

  GtkTreeIter gtkIter;
};

}
