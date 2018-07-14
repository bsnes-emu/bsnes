#if defined(Hiro_TableView)

namespace hiro {

struct pTableViewColumn : pObject {
  Declare(TableViewColumn, Object)

  auto setActive() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setExpandable(bool expandable) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setHorizontalAlignment(double alignment) -> void;
  auto setIcon(const image& icon) -> void;
  auto setParent(mObject* parent, int offset) -> void override;
  auto setResizable(bool resizable) -> void;
  auto setSortable(bool sortable) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double alignment) -> void;
  auto setVisible(bool visible) -> void;
  auto setWidth(signed width) -> void;

  auto _parent() -> maybe<pTableViewHeader&>;
  auto _setState() -> void;
};

}

#endif
