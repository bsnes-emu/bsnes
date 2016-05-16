#if defined(Hiro_ComboEdit)

namespace hiro {

struct pComboEditItem : pObject {
  Declare(ComboEditItem, Object)

  auto setIcon(const image& icon) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> maybe<pComboEdit&>;

  GtkTreeIter gtkIter;
};

}

#endif
