#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewColumn : pObject {
  Declare(TableViewColumn, Object)

  auto setActive() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setExpandable(bool expandable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setHorizontalAlignment(double alignment) -> void;
  auto setIcon(const image& icon) -> void;
  auto setResizable(bool resizable) -> void;
  auto setSorting(Sort sorting) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double alignment) -> void;
  auto setWidth(signed width) -> void;

  auto _parent() -> maybe<pTableView&>;
  auto _setState() -> void;

  signed _width = 128;  //computed width (via TableView::resizeColumns)
};

}

#endif
