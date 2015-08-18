#if defined(Hiro_MenuRadioItem)

namespace hiro {

struct pMenuRadioItem : pAction {
  Declare(MenuRadioItem, Action)

  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void override;

  QtMenuRadioItem* qtMenuRadioItem = nullptr;
  QActionGroup* qtActionGroup = nullptr;
};

}

#endif
