#if defined(Hiro_Widget)
struct mWidget : mSizable {
  Declare(Widget)

  auto doDrop(vector<string> names) const -> void;
  auto doMouseEnter() const -> void;
  auto doMouseLeave() const -> void;
  auto doMouseMove(Position position) const -> void;
  auto doMousePress(Mouse::Button button) const -> void;
  auto doMouseRelease(Mouse::Button button) const -> void;
  auto droppable() const -> bool;
  auto focusable() const -> bool;
  auto mouseCursor() const -> MouseCursor;
  auto onDrop(const function<void (vector<string>)>& callback = {}) -> type&;
  auto onMouseEnter(const function<void ()>& callback = {}) -> type&;
  auto onMouseLeave(const function<void ()>& callback = {}) -> type&;
  auto onMouseMove(const function<void (Position position)>& callback = {}) -> type&;
  auto onMousePress(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto onMouseRelease(const function<void (Mouse::Button)>& callback = {}) -> type&;
  auto remove() -> type& override;
  auto setDroppable(bool droppable = true) -> type&;
  auto setFocusable(bool focusable = true) -> type&;
  auto setMouseCursor(const MouseCursor& mouseCursor = {}) -> type&;
  auto setToolTip(const string& toolTip = "") -> type&;
  auto toolTip() const -> string;

//private:
  struct State {
    bool droppable = false;
    bool focusable = false;
    MouseCursor mouseCursor;
    function<void (vector<string>)> onDrop;
    function<void ()> onMouseEnter;
    function<void ()> onMouseLeave;
    function<void (Position)> onMouseMove;
    function<void (Mouse::Button)> onMousePress;
    function<void (Mouse::Button)> onMouseRelease;
    string toolTip;
  } state;
};
#endif
