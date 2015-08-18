#if defined(Hiro_RadioLabel)

namespace hiro {

struct pRadioLabel : pWidget {
  Declare(RadioLabel, Widget)

  auto minimumSize() const -> Size;
  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void;
  auto setText(const string& text) -> void;

  auto _setState() -> void;

  QtRadioLabel* qtRadioLabel = nullptr;
};

}

#endif
