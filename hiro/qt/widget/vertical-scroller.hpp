#if defined(Hiro_VerticalScroller)

namespace hiro {

struct pVerticalScroller : pWidget {
  Declare(VerticalScroller, Widget)

  auto minimumSize() const -> Size override;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;

  auto _setState() -> void;

  QtVerticalScroller* qtVerticalScroller = nullptr;
};

}

#endif
