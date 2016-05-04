#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewItem : pObject {
  Declare(TableViewItem, Object)

  auto append(sTableViewCell cell) -> void;
  auto remove(sTableViewCell cell) -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setSelected(bool selected) -> void;

  auto _parent() -> maybe<pTableView&>;
  auto _setState() -> void;

  QTreeWidgetItem* qtItem = nullptr;
};

}

#endif
