#if defined(Hiro_Canvas)

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size override;
  auto setColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Gradient gradient) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  auto _paint() -> void;
  auto _rasterize() -> void;
  auto _redraw() -> void;

  vector<uint32_t> pixels;
  signed width = 0;
  signed height = 0;
};

}

#endif
