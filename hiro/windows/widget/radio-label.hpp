#if defined(Hiro_RadioLabel)

namespace hiro {

struct pRadioLabel : pWidget {
  Declare(RadioLabel, Widget)

  auto minimumSize() const -> Size override;
  auto setChecked() -> void;
  auto setGroup(sGroup group) -> void override;
  auto setText(const string& text) -> void;

  auto onActivate() -> void;
};

}

#endif
