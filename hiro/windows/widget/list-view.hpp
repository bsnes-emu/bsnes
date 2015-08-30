#if defined(Hiro_ListView)

namespace hiro {

struct pListView : pWidget {
  Declare(ListView, Widget)

  auto append(sListViewHeader header) -> void;
  auto append(sListViewItem item) -> void;
  auto remove(sListViewHeader header) -> void;
  auto remove(sListViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void override;

  auto onActivate(LPARAM lparam) -> void;
  auto onChange(LPARAM lparam) -> void;
  auto onContext(LPARAM lparam) -> void;
  auto onCustomDraw(LPARAM lparam) -> LRESULT;
  auto onSort(LPARAM lparam) -> void;
  auto onToggle(LPARAM lparam) -> void;

  auto _backgroundColor(unsigned row, unsigned column) -> Color;
  auto _cellWidth(unsigned row, unsigned column) -> unsigned;
  auto _columnWidth(unsigned column) -> unsigned;
  auto _font(unsigned row, unsigned column) -> Font;
  auto _foregroundColor(unsigned row, unsigned column) -> Color;
  auto _setIcons() -> void;
  auto _setSortable() -> void;
  auto _width(unsigned column) -> unsigned;

  WindowProc windowProc = nullptr;
  HIMAGELIST imageList = 0;
  vector<image> icons;
};

}

#endif
