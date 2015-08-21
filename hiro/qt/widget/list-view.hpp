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

  auto _onSize() -> void;
  auto _width(unsigned column) -> unsigned;
  auto _widthOfColumn(unsigned column) -> unsigned;
  auto _widthOfCell(unsigned row, unsigned column) -> unsigned;

  QtListView* qtListView = nullptr;
  QtListViewDelegate* qtListViewDelegate = nullptr;
};

}

#endif
