#if defined(Hiro_HorizontalSlider)

namespace hiro {

struct pHorizontalSlider : pWidget {
  Declare(HorizontalSlider, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;
};

}

#endif
