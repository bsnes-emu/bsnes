#if defined(Hiro_ComboButton)

namespace hiro {

struct pComboButton : pWidget {
  Declare(ComboButton, Widget)

  auto append(sComboButtonItem item) -> void;
  auto minimumSize() const -> Size override;
  auto remove(sComboButtonItem item) -> void;
  auto reset() -> void;
  auto setFont(const Font& font) -> void override;

  auto _append(sComboButtonItem item) -> void;
  auto _updateSelected() -> void;

  GtkListStore* gtkListStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  GtkComboBox* gtkComboBox = nullptr;
  GtkCellRenderer* gtkCellIcon = nullptr;
  GtkCellRenderer* gtkCellText = nullptr;
};

}

#endif
