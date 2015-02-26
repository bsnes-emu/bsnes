namespace hiro {

struct pIconViewItem : pObject {
  Declare(IconViewItem, Object)

  auto setIcon(const image& icon) -> void;
  auto setSelected(bool selected) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pIconView*;
};

}
