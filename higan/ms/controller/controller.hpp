struct Controller : Thread {
  Controller(uint port);
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint7 { return 0x7f; }

  const uint port;
};

#include "gamepad/gamepad.hpp"
