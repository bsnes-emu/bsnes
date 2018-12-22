struct Controller : Thread {
  Controller(uint port);
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint8 { return 0xff; }
  virtual auto writeData(uint8 data) -> void {}

  const uint port;
};

struct ControllerPort {
  auto connect(uint deviceID) -> void;

  auto power(uint port) -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  uint port;
  Controller* device = nullptr;
};

extern ControllerPort controllerPort1;
extern ControllerPort controllerPort2;

#include "numberpad/numberpad.hpp"
#include "gamepad/gamepad.hpp"
