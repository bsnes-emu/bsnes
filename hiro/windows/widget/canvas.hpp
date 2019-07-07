#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size override;
  auto setAlignment(Alignment) -> void;
  auto setColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void override;
  auto setFocusable(bool focusable) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Gradient gradient) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  auto doMouseLeave() -> void override;
  auto doMouseMove(int x, int y) -> void override;
  auto windowProc(HWND, UINT, WPARAM, LPARAM) -> maybe<LRESULT> override;

  auto _paint() -> void;
  auto _rasterize() -> void;
  auto _redraw() -> void;

  vector<uint32_t> pixels;
  signed width = 0;
  signed height = 0;
};

}

#endif
