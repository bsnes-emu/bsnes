struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, B, A, Select, Start,
  };

  Gamepad(bool port);
  auto data() -> uint3;
  auto latch(bool data) -> void;

private:
  bool latched = 0;
  uint counter = 0;

  bool a = 0;
  bool b = 0;
  bool select = 0;
  bool start = 0;
  bool up = 0;
  bool down = 0;
  bool left = 0;
  bool right = 0;
};
