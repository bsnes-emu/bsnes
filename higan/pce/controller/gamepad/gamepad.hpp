struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, Two, One, Select, Run,
  };

  Gamepad();

  auto readData() -> uint8 override;
};
