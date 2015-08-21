#if defined(Hiro_HorizontalScrollBar)

namespace hiro {

struct pHorizontalScrollBar : pWidget {
  Declare(HorizontalScrollBar, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto onChange(WPARAM wparam) -> void;
};

}

#endif
