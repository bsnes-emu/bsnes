#if defined(Hiro_ComboEdit)

namespace hiro {

struct pComboEdit : pWidget {
  Declare(ComboEdit, Widget)

  auto append(sComboEditItem item) -> void;
  auto minimumSize() const -> Size override;
  auto remove(sComboEditItem item) -> void;
  auto reset() -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setFont(const Font& font) -> void override;
  auto setText(const string& text) -> void;

  auto _updateText() -> void;

  GtkListStore* gtkListStore = nullptr;
  GtkTreeModel* gtkTreeModel = nullptr;
  GtkComboBox* gtkComboBox = nullptr;
  GtkCellRenderer* gtkCellIcon = nullptr;
  GtkCellRenderer* gtkCellText = nullptr;
};

}

#endif
