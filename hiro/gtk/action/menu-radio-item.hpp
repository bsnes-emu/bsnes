#if defined(Hiro_MenuRadioItem)

namespace hiro {

struct pMenuRadioItem : pAction {
  Declare(MenuRadioItem, Action)

  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void;
  auto setText(const string& text) -> void;

  auto groupLocked() const -> bool;

  auto _doActivate() -> void;

  GtkCheckMenuItem* gtkCheckMenuItem = nullptr;
  GtkRadioMenuItem* gtkRadioMenuItem = nullptr;
};

}

#endif
