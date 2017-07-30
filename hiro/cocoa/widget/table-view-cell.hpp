#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewCell : pObject {
  Declare(TableViewCell, Object)

  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setCheckable(bool checkable) -> void;
  auto setChecked(bool checked) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto _grandparent() -> maybe<pTableView&>;
  auto _parent() -> maybe<pTableViewItem&>;
  auto _redraw() -> void;
};

}

#endif
