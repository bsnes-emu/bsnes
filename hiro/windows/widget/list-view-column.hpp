#if defined(Hiro_ListView)

namespace hiro {

struct pListViewColumn : pObject {
  Declare(ListViewColumn, Object)

  auto setActive() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setExpandable(bool expandable) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setHorizontalAlignment(double alignment) -> void;
  auto setImage(const Image& image) -> void;
  auto setResizable(bool resizable) -> void;
  auto setSortable(bool sortable) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double alignment) -> void;
  auto setWidth(signed width) -> void;

  auto _grandparent() -> maybe<pListView&>;
  auto _parent() -> maybe<pListViewHeader&>;
  auto _setState() -> void;

  signed _width = 128;  //computed width (via ListView::resizeColumns)
};

}

#endif
