#if defined(Hiro_Viewport)

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr_t;
  auto setDroppable(bool droppable) -> void;

  auto doMouseLeave() -> void override;
  auto doMouseMove(int x, int y) -> void override;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT> override;
};

}

#endif
