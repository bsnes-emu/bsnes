#if defined(Hiro_ComboButton)

namespace hiro {

struct pComboButtonItem : pObject {
  Declare(ComboButtonItem, Object)

  auto setImage(const Image& icon) -> void;
  auto setSelected() -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pComboButton*;

  GtkTreeIter gtkIter;
};

}

#endif
