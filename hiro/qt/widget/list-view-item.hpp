#if defined(Hiro_ListView)

namespace hiro {

struct pListViewItem : pObject {
  Declare(ListViewItem, Object)

  auto append(sListViewCell cell) -> void;
  auto remove(sListViewCell cell) -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setSelected(bool selected) -> void;

  auto _parent() -> maybe<pListView&>;
  auto _setState() -> void;

  QTreeWidgetItem* qtItem = nullptr;
};

}

#endif
