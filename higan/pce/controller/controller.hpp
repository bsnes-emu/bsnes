struct Controller : Thread {
  Controller();
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint4 { return 0; }
  virtual auto writeData(uint2) -> void {}
};

#include "gamepad/gamepad.hpp"
