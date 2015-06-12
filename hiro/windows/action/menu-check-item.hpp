#if defined(Hiro_MenuCheckItem)

namespace hiro {

struct pMenuCheckItem : pAction {
  Declare(MenuCheckItem, Action)

  auto setChecked(bool checked) -> void;
  auto setText(const string& text) -> void;

  auto onToggle() -> void;
};

}

#endif
