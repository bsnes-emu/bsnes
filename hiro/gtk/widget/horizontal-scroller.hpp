namespace hiro {

struct pHorizontalScroller : pWidget {
  Declare(HorizontalScroller, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;
};

}
