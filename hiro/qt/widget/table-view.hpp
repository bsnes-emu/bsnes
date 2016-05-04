#if defined(Hiro_TableView)

namespace hiro {

struct pTableView : pWidget {
  Declare(TableView, Widget)

  auto append(sTableViewHeader header) -> void;
  auto append(sTableViewItem item) -> void;
  auto remove(sTableViewHeader header) -> void;
  auto remove(sTableViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setForegroundColor(Color color) -> void;

  auto _onSize() -> void;
  auto _width(unsigned column) -> unsigned;
  auto _widthOfColumn(unsigned column) -> unsigned;
  auto _widthOfCell(unsigned row, unsigned column) -> unsigned;

  QtTableView* qtTableView = nullptr;
  QtTableViewDelegate* qtTableViewDelegate = nullptr;
};

}

#endif
