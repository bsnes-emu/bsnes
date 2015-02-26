namespace hiro {

struct pMenuRadioItem : pAction {
  Declare(MenuRadioItem, Action)

  auto setChecked() -> void;
  auto setGroup(const vector<shared_pointer_weak<mMenuRadioItem>>& group) -> void;
  auto setText(const string& text) -> void;

  auto _doActivate() -> void;
  auto _parent() -> pMenuRadioItem&;
};

}
