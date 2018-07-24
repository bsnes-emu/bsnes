#if defined(Hiro_Viewport)

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr;
  auto setDroppable(bool droppable) -> void;
};

}

#endif
