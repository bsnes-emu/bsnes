#if defined(Hiro_TreeView)

namespace hiro {

struct pTreeView : pWidget {
  Declare(TreeView, Widget)

  auto append(sTreeViewItem item) -> void;
  auto remove(sTreeViewItem item) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFocused() -> void override;
  auto setForegroundColor(Color color) -> void;

  auto _activatePath(GtkTreePath* gtkPath) -> void;
  auto _buttonEvent(GdkEventButton* gdkEvent) -> signed;
  auto _doDataFunc(GtkTreeViewColumn* column, GtkCellRenderer* renderer, GtkTreeIter* iter) -> void;
  auto _togglePath(string path) -> void;
  auto _updateSelected() -> void;

  GtkScrolledWindow* gtkScrolledWindow = nullptr;
  GtkWidget* gtkWidgetChild = nullptr;
  GtkTreeStore* gtkTreeStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  GtkTreeSelection* gtkTreeSelection = nullptr;
  GtkTreeView* gtkTreeView = nullptr;
  GtkTreeViewColumn* gtkTreeViewColumn = nullptr;
  GtkCellRenderer* gtkCellToggle = nullptr;
  GtkCellRenderer* gtkCellPixbuf = nullptr;
  GtkCellRenderer* gtkCellText = nullptr;
};

}

#endif
