struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, One, Two,
  };

  Gamepad(uint port);

  auto readData() -> uint8 override;
};
