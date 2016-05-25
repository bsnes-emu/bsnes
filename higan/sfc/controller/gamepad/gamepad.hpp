struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, B, A, Y, X, L, R, Select, Start,
  };

  Gamepad(bool port);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;

  bool b, y, select, start;
  bool up, down, left, right;
  bool a, x, l, r;
};
