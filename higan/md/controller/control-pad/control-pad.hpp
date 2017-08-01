struct ControlPad : Controller {
  enum : uint {
    Up, Down, Left, Right, A, B, C, Start,
  };

  ControlPad(uint port);

  auto readData() -> uint8 override;
  auto writeData(uint8 data) -> void override;

  uint1 select = 1;
  uint1 latch;
};
