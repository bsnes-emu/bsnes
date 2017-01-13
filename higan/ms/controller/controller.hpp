struct Controller : Thread {
  Controller(uint port);
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint8 { return 0xff; }

  const uint port;
};

#include "mastersystem/mastersystem.hpp"
#include "gamegear/gamegear.hpp"
#include "gamepad/gamepad.hpp"
