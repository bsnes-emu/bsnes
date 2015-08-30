#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size;
  auto setColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Gradient gradient) -> void;
  auto setImage(const Image& image) -> void;
  auto update() -> void;

  auto _onExpose(GdkEventExpose* event) -> void;
  auto _rasterize() -> void;
  auto _redraw() -> void;
  auto _release() -> void;

  GdkPixbuf* surface = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;
};

}

#endif
