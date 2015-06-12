#if defined(Hiro_MenuRadioItem)

namespace hiro {

struct pMenuRadioItem : pAction {
  Declare(MenuRadioItem, Action)

  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void override;
  auto setText(const string& text) -> void;

  auto onActivate() -> void;
};

}

#endif
