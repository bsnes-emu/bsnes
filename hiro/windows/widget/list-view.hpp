#if defined(Hiro_ListView)

namespace hiro {

struct pListView : pWidget {
  Declare(ListView, Widget)

  auto append(sListViewColumn column) -> void;
  auto append(sListViewItem item) -> void;
  auto checkAll() -> void;
  auto remove(sListViewColumn column) -> void;
  auto remove(sListViewItem item) -> void;
  auto reset() -> void;
  auto resizeColumns() -> void;
  auto selectAll() -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setCheckable(bool checkable) -> void;
  auto setChecked(bool checked) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setGridVisible(bool visible) -> void;
  auto setHeaderVisible(bool visible) -> void;
  auto setSortable(bool sortable) -> void;
  auto uncheckAll() -> void;
  auto unselectAll() -> void;

  auto onActivate(LPARAM lparam) -> void;
  auto onChange(LPARAM lparam) -> void;
  auto onContext(LPARAM lparam) -> void;
  auto onCustomDraw(LPARAM lparam) -> LRESULT;
  auto onSort(LPARAM lparam) -> void;

  auto _backgroundColor(unsigned row, unsigned column) -> Color;
  auto _cellWidth(unsigned row, unsigned column) -> unsigned;
  auto _columnWidth(unsigned column) -> unsigned;
  auto _font(unsigned row, unsigned column) -> string;
  auto _foregroundColor(unsigned row, unsigned column) -> Color;
  auto _setIcons() -> void;
  auto _width(unsigned column) -> unsigned;

  HIMAGELIST imageList = 0;
  vector<image> icons;
};

}

#endif
