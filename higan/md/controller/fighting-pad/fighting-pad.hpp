struct FightingPad : Controller {
  enum : uint {
    Up, Down, Left, Right, A, B, C, X, Y, Z, Mode, Start,
  };

  FightingPad(uint port);
  auto main() -> void override;

  auto readData() -> uint8 override;
  auto writeData(uint8 data) -> void override;

  uint1 select = 1;
  uint1 latch;
  uint3 counter;
  uint32 timeout;
};
