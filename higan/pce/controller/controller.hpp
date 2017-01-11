struct Controller : Thread {
  Controller();
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint8 { return 0x00; }
};

#include "gamepad/gamepad.hpp"
