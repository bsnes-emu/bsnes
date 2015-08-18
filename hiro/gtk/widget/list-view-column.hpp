#if defined(Hiro_ListView)

namespace hiro {

struct pListViewColumn : pObject {
  Declare(ListViewColumn, Object)

  auto setActive() -> void;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setEditable(bool editable) -> void;
  auto setExpandable(bool expandable) -> void;
  auto setFont(const string& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setHorizontalAlignment(double) -> void {}
  auto setIcon(const image& icon) -> void;
  auto setResizable(bool resizable) -> void;
  auto setSortable(bool sortable) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double) -> void {}
  auto setVisible(bool visible) -> void override;
  auto setWidth(signed width) -> void;

  auto _grandparent() -> maybe<pListView&>;
  auto _parent() -> maybe<pListViewHeader&>;
  auto _setState() -> void;

  GtkTreeViewColumn* gtkColumn = nullptr;
  GtkWidget* gtkHeader = nullptr;
  GtkWidget* gtkHeaderIcon = nullptr;
  GtkWidget* gtkHeaderText = nullptr;
  GtkCellRenderer* gtkCellToggle = nullptr;
  GtkCellRenderer* gtkCellIcon = nullptr;
  GtkCellRenderer* gtkCellText = nullptr;
};

}

#endif
