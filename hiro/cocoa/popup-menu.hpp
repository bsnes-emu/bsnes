#if defined(Hiro_PopupMenu)

namespace hiro {

struct pPopupMenu : pObject {
  Declare(PopupMenu, Object)

  auto append(sAction action) -> void;
  auto remove(sAction action) -> void;
  auto setVisible(bool visible) -> void;
};

}

#endif
