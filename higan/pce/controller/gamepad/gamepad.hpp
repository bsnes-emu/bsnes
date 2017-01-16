struct Gamepad : Controller {
  enum : uint {
    Up, Down, Left, Right, Two, One, Select, Run,
  };

  Gamepad();

  auto readData() -> uint4 override;
  auto writeData(uint2 data) -> void override;

  bool sel;
  bool clr;
};
