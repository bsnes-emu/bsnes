namespace hiro {

struct pVerticalScroller : pWidget {
  Declare(VerticalScroller, Widget)

  auto minimumSize() const -> Size;
  auto setLength(unsigned length) -> void;
  auto setPosition(unsigned position) -> void;
};

}
