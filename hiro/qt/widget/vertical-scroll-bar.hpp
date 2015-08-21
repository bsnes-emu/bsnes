#if defined(Hiro_VerticalScrollBar)

namespace hiro {

struct pVerticalScrollBar : pWidget {
  Declare(VerticalScrollBar, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto _setState() -> void;

  QtVerticalScrollBar* qtVerticalScrollBar = nullptr;
};

}

#endif
