namespace hiro {

struct pComboButton : pWidget {
  Declare(ComboButton, Widget)

  auto append(sComboButtonItem item) -> void;
  auto minimumSize() const -> Size override;
  auto remove(sComboButtonItem item) -> void;
  auto reset() -> void;
  auto setFont(const string& font) -> void override;

  auto _updateSelected() -> void;

  GtkListStore* gtkListStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  GtkComboBox* gtkComboBox = nullptr;
  GtkCellRenderer* gtkCellIcon = nullptr;
  GtkCellRenderer* gtkCellText = nullptr;
};

}
