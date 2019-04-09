#if defined(Hiro_Canvas)
struct mCanvas : mWidget {
  Declare(Canvas)

  auto alignment() const -> Alignment;
  auto color() const -> Color;
  auto data() -> uint32_t*;
  auto droppable() const -> bool;
  auto doDrop(vector<string> names) const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto gradient() const -> Gradient;
  auto icon() const -> image;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onMouseLeave(const function<void ()>& callback = {}) -> type&;
  auto onMouseMove(const function<void (Position)>& callback = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto setAlignment(Alignment alignment = {}) -> type&;
  auto setColor(Color color = {}) -> type&;
  auto setDroppable(bool droppable = true) -> type&;
  auto setGradient(Gradient gradient = {}) -> type&;
  auto setIcon(const image& icon = {}) -> type&;
  auto setSize(Size size = {}) -> type&;
  auto size() const -> Size;
  auto update() -> type&;

//private:
  struct State {
    Alignment alignment;
    Color color;
    bool droppable = false;
    Gradient gradient;
    image icon;
    function<void (vector<string>)> onDrop;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
  } state;
};
#endif
