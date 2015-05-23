#if defined(Hiro_RadioButton)

namespace hiro {

struct pRadioButton : pWidget {
  Declare(RadioButton, Widget)

  auto minimumSize() const -> Size;
  auto setBordered(bool bordered) -> void;
  auto setChecked() -> void;
  auto setGroup(const vector<wRadioButton>& group) -> void;
  auto setIcon(const image& icon) -> void;
  auto setOrientation(Orientation orientation) -> void;
  auto setText(const string& text) -> void;

  auto _parent() -> pRadioButton&;
};

}

#endif
