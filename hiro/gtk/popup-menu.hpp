#if defined(Hiro_PopupMenu)

namespace hiro {

struct pPopupMenu : pObject {
  Declare(PopupMenu, Object)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setFont(const Font& font) -> void override;
  auto setVisible(bool visible) -> void;

  GtkWidget* gtkMenu = nullptr;
};

}

#endif
