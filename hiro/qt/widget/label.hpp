#if defined(Hiro_Label)

namespace hiro {

struct pLabel : pWidget {
  Declare(Label, Widget)

  auto minimumSize() const -> Size override;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setFont(const Font& font) -> void override;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;

  QtLabel* qtLabel = nullptr;
};

}

#endif
