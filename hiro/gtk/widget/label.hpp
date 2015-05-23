#if defined(Hiro_Label)

namespace hiro {

struct pLabel : pWidget {
  Declare(Label, Widget)

  auto minimumSize() const -> Size override;
  auto setHorizontalAlignment(double alignment) -> void;
  auto setText(const string& text) -> void;
  auto setVerticalAlignment(double alignment) -> void;

  auto _setAlignment() -> void;
};

}

#endif
