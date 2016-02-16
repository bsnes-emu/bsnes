struct Gamepad : Controller {
  enum : uint {
    B, Y, Select, Start, Up, Down, Left, Right, A, X, L, R,
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
