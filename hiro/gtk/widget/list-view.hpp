#if defined(Hiro_ListView)

namespace hiro {

struct pListView : pWidget {
  Declare(ListView, Widget)

  auto append(sListViewHeader column) -> void;
  auto append(sListViewItem item) -> void;
  auto focused() const -> bool override;
  auto remove(sListViewHeader column) -> void;
  auto remove(sListViewItem item) -> void;
  auto resizeColumns() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setBatchable(bool batchable) -> void;
  auto setBordered(bool bordered) -> void;
  auto setFocused() -> void override;
  auto setFont(const string& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setGeometry(Geometry geometry) -> void override;

  auto _cellWidth(unsigned row, unsigned column) -> unsigned;
  auto _columnWidth(unsigned column) -> unsigned;
  auto _createModel() -> void;
  auto _doActivate() -> void;
  auto _doChange() -> void;
  auto _doContext() -> void;
  auto _doDataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void;
  auto _doEdit(GtkCellRendererText* gtkCellRendererText, const char* path, const char* text) -> void;
  auto _doEvent(GdkEventButton* event) -> signed;
  auto _doHeaderActivate(GtkTreeViewColumn* column) -> void;
  auto _doMouseMove() -> signed;
  auto _doToggle(GtkCellRendererToggle* gtkCellRendererToggle, const char* path) -> void;
  auto _updateSelected() -> void;
  auto _width(unsigned column) -> unsigned;

  GtkScrolledWindow* gtkScrolledWindow = nullptr;
  GtkWidget* gtkWidgetChild = nullptr;
  GtkTreeView* gtkTreeView = nullptr;
  GtkTreeSelection* gtkTreeSelection = nullptr;
  GtkListStore* gtkListStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  vector<unsigned> currentSelection;
};

}

#endif
