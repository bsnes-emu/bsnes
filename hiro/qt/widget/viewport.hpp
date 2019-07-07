#if defined(Hiro_Viewport)

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr_t;
  auto setDroppable(bool droppable) -> void override;
  auto setFocusable(bool focusable) -> void override;

  auto _setState() -> void;

  QtViewport* qtViewport = nullptr;
};

}

#endif
