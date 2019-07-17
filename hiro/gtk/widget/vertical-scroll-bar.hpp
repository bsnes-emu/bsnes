#if defined(Hiro_VerticalScrollBar)

namespace hiro {

struct pVerticalScrollBar : pWidget {
  Declare(VerticalScrollBar, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;
};

}

#endif
