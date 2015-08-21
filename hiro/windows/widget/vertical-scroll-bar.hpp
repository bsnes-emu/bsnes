#if defined(Hiro_VerticalScrollBar)

namespace hiro {

struct pVerticalScrollBar : pWidget {
  Declare(VerticalScrollBar, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto onChange(WPARAM wparam) -> void;
};

}

#endif
