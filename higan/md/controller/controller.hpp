struct Controller : Thread {
  Controller(uint port);
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint8 { return 0xff; }
  virtual auto writeData(uint8 data) -> void {}

  virtual auto readControl() -> uint8 { return 0x00; }
  virtual auto writeControl(uint8 data) -> void {}

  const uint port;
};

#include "gamepad/gamepad.hpp"
