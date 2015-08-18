#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size;
  auto setColor(Color color) -> void;
  auto setData(Size size) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void;
  auto setGradient(Color topLeft, Color topRight, Color bottomLeft, Color bottomRight) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  enum class Mode : unsigned { Color, Data, Gradient, Icon };

  auto _rasterize() -> void;
  auto _release() -> void;

  QtCanvas* qtCanvas = nullptr;
  QImage* qtImage = nullptr;
  unsigned qtImageWidth = 0;
  unsigned qtImageHeight = 0;
  Mode mode = Mode::Color;
};

}

#endif
