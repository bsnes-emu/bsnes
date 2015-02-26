namespace hiro {

struct pMenuCheckItem : pAction {
  Declare(MenuCheckItem, Action)
  auto orphan() -> void;

  auto setChecked(bool checked) -> void;
  auto setText(const string& text) -> void;
};

}
