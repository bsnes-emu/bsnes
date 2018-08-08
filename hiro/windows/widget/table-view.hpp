#if defined(Hiro_TableView)

namespace hiro {

struct pTableView : pWidget {
  Declare(TableView, Widget)

  auto append(sTableViewColumn column) -> void;
  auto append(sTableViewItem item) -> void;
  auto remove(sTableViewColumn column) -> void;
  auto remove(sTableViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setHeadered(bool headered) -> void;
  auto setSortable(bool sortable) -> void;

  auto onActivate(LPARAM lparam) -> void;
  auto onChange(LPARAM lparam) -> void;
  auto onContext(LPARAM lparam) -> void;
  auto onCustomDraw(LPARAM lparam) -> LRESULT;
  auto onSort(LPARAM lparam) -> void;
  auto onToggle(LPARAM lparam) -> void;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT> override;

  auto _backgroundColor(unsigned row, unsigned column) -> Color;
  auto _cellWidth(unsigned row, unsigned column) -> unsigned;
  auto _columnWidth(unsigned column) -> unsigned;
  auto _font(unsigned row, unsigned column) -> Font;
  auto _foregroundColor(unsigned row, unsigned column) -> Color;
  auto _setIcons() -> void;
  auto _width(unsigned column) -> unsigned;

  HIMAGELIST imageList = 0;
  vector<image> icons;
};

}

#endif
