#if defined(Hiro_IconView)

namespace hiro {

struct pIconViewItem : pObject {
  Declare(IconViewItem, Object)

  auto setImage(const Image& image) -> void;
  auto setSelected(bool selected) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pIconView*;
};

}

#endif
