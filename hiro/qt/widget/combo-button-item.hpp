#if defined(Hiro_ComboButton)

namespace hiro {

struct pComboButtonItem : pObject {
  Declare(ComboButtonItem, Object)

  auto setIcon(const image& icon) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> maybe<pComboButton&>;
  auto _setState() -> void;
};

}

#endif
