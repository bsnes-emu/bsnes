struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, A, B, C, Start,
  };

  Gamepad(uint port);

  auto readData() -> uint8 override;
  auto writeData(uint8 data) -> void override;

  boolean select;
  boolean latch;
};
