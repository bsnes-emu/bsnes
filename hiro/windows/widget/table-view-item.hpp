#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewItem : pObject {
  Declare(TableViewItem, Object)

  auto append(sTableViewCell cell) -> void;
  auto remove(sTableViewCell cell) -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFocused() -> void;
  auto setForegroundColor(Color color) -> void;
  auto setSelected(bool selected) -> void;

  auto _parent() -> maybe<pTableView&>;
  auto _setState() -> void;
};

}

#endif
