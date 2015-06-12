#if defined(Hiro_VerticalSlider)

namespace hiro {

struct pVerticalSlider : pWidget {
  Declare(VerticalSlider, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto onChange() -> void;
};

}

#endif
