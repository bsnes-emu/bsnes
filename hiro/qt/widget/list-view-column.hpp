#if defined(Hiro_ListView)

namespace hiro {

struct pListViewColumn : pObject {
  Declare(ListViewColumn, Object)

  auto setActive() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setExpandable(bool expandable) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setHorizontalAlignment(double alignment) -> void;
  auto setIcon(const image& icon) -> void;
  auto setResizable(bool resizable) -> void;
  auto setSortable(bool sortable) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double alignment) -> void;
  auto setVisible(bool visible) -> void;
  auto setWidth(signed width) -> void;

  auto _parent() -> maybe<pListViewHeader&>;
  auto _setState() -> void;
};

}

#endif
