#if defined(Hiro_VerticalSlider)

namespace hiro {

struct pVerticalSlider : pWidget {
  Declare(VerticalSlider, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto _setState() -> void;

  QtVerticalSlider* qtVerticalSlider = nullptr;
};

}

#endif
