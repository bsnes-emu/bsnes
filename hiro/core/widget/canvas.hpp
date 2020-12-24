#if defined(Hiro_Canvas)
struct mCanvas : mWidget {
  Declare(Canvas)

  auto alignment() const -> Alignment;
  auto color() const -> Color;
  auto data() -> uint32_t*;
  auto gradient() const -> Gradient;
  auto icon() const -> image;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setColor(Color color = {}) -> type&;
  auto setGradient(Gradient gradient = {}) -> type&;
  auto setIcon(const image& icon = {}, Color padding = {0,0,0,0}) -> type&;
  auto setSize(Size size = {}) -> type&;
  auto size() const -> Size;
  auto update() -> type&;

//private:
  struct State {
    Alignment alignment;
    Color color;
    Gradient gradient;
    image icon;
  } state;
};
#endif
