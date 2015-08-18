#if defined(Hiro_Label)

namespace hiro {

struct pLabel : pWidget {
  Declare(Label, Widget)

  auto minimumSize() const -> Size override;
  auto setAlignment(Alignment alignment) -> void;
  auto setText(const string& text) -> void;
};

}

#endif
