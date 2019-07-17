#if defined(Hiro_HorizontalScrollBar)

namespace hiro {

struct pHorizontalScrollBar : pWidget {
  Declare(HorizontalScrollBar, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;
};

}

#endif
