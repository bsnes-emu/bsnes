#if defined(Hiro_ListView)

namespace hiro {

struct pListView : pWidget {
  Declare(ListView, Widget)

  auto append(sListViewColumn column) -> void;
  auto append(sListViewItem item) -> void;
  auto focused() -> bool;
  auto remove(sListViewColumn column) -> void;
  auto remove(sListViewItem item) -> void;
  auto reset() -> void;
  auto resizeColumns() -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setCheckable(bool checkable) -> void;
  auto setChecked(bool checked) -> void;
  auto setFocused() -> void override;
  auto setFont(const string& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setGridVisible(bool visible) -> void;
  auto setHeaderVisible(bool visible) -> void;
  auto setMultiSelect(bool multiSelect) -> void;
  auto setSelected(bool selected) -> void;

  auto _column(unsigned column) -> pListViewColumn*;
  auto _createModel() -> void;
  auto _doActivate() -> void;
  auto _doChange() -> void;
  auto _doContext() -> void;
  auto _doEdit(GtkCellRendererText* renderer, const char* path, const char* text) -> void;
  auto _doEvent(GdkEventButton* event) -> signed;
  auto _doHeaderActivate(GtkTreeViewColumn* column) -> void;
  auto _doMouseMove() -> signed;
  auto _doToggle(const char* path) -> void;
  auto _updateSelected() -> void;

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
