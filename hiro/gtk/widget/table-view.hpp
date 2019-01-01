#if defined(Hiro_TableView)

namespace hiro {

struct pTableView : pWidget {
  Declare(TableView, Widget)

  auto append(sTableViewColumn column) -> void;
  auto append(sTableViewItem item) -> void;
  auto focused() const -> bool override;
  auto remove(sTableViewColumn column) -> void;
  auto remove(sTableViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setFocused() -> void override;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setHeadered(bool headered) -> void;
  auto setSortable(bool sortable) -> void;

  auto _cellWidth(uint row, uint column) -> uint;
  auto _columnWidth(uint column) -> uint;
  auto _createModel() -> void;
  auto _doActivate() -> void;
  auto _doChange() -> void;
  auto _doContext() -> void;
  auto _doDataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void;
  auto _doEdit(GtkCellRendererText* gtkCellRendererText, const char* path, const char* text) -> void;
  auto _doEvent(GdkEventButton* event) -> int;
  auto _doHeaderActivate(GtkTreeViewColumn* column) -> void;
  auto _doMouseMove() -> int;
  auto _doToggle(GtkCellRendererToggle* gtkCellRendererToggle, const char* path) -> void;
  auto _updateSelected() -> void;
  auto _width(uint column) -> uint;

  GtkScrolledWindow* gtkScrolledWindow = nullptr;
  GtkWidget* gtkWidgetChild = nullptr;
  GtkTreeView* gtkTreeView = nullptr;
  GtkTreeSelection* gtkTreeSelection = nullptr;
  GtkListStore* gtkListStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  vector<uint> currentSelection;
  bool suppressChange = false;
};

}

#endif
