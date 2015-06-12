#if defined(Hiro_ListView)

namespace hiro {

struct pListViewItem : pObject {
  Declare(ListViewItem, Object)

  auto append(sListViewCell cell) -> void;
  auto remove(sListViewCell cell) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setChecked(bool checked) -> void;
  auto setFocused() -> void;
  auto setForegroundColor(Color color) -> void;
  auto setSelected(bool selected) -> void;

  auto _parent() -> pListView*;

  GtkTreeIter gtkIter;
};

}

#endif
