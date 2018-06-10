#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size;
  auto setColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Gradient gradient) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  auto _onDraw(cairo_t* context) -> void;
  auto _onExpose(GdkEventExpose* event) -> void;
  auto _rasterize() -> void;
  auto _redraw() -> void;
  auto _release() -> void;

  GdkPixbuf* surface = nullptr;
  uint surfaceWidth = 0;
  uint surfaceHeight = 0;
};

}

#endif
