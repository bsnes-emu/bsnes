namespace hiro {

struct pRadioLabel : pWidget {
  Declare(RadioLabel, Widget)

  auto minimumSize() const -> Size;
  auto setChecked() -> void;
  auto setGroup(const vector<shared_pointer_weak<mRadioLabel>>& group) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pRadioLabel&;
};

}
