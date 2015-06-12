#if defined(Hiro_PopupMenu)

namespace hiro {

struct pPopupMenu : pObject {
  Declare(PopupMenu, Object)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setFont(const string& font) -> void override;
  auto setVisible(bool visible) -> void override;

  HWND hwnd = nullptr;
  HMENU hmenu = nullptr;
};

}

#endif
