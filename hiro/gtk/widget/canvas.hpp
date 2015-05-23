#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size;
  auto setColor(Color color) -> void;
  auto setData(Size size) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  enum class Mode : unsigned { Color, Data, Gradient, Icon };

  auto _onExpose(GdkEventExpose* event) -> void;
  auto _rasterize() -> void;
  auto _redraw() -> void;
  auto _release() -> void;

  GdkPixbuf* surface = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;
  Mode mode = Mode::Color;
};

}

#endif
