struct Gamepad : Controller {
  Gamepad(bool port);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  unsigned counter;

  bool b, y, select, start;
  bool up, down, left, right;
  bool a, x, l, r;
};
