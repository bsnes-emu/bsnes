struct Controller : Thread {
  Controller();
  virtual ~Controller();

  static auto Enter() -> void;
  auto main() -> void;

  virtual auto readData() -> uint4 { return 0x0f; }
  virtual auto writeData(uint2) -> void {}
};

struct ControllerPort {
  auto connect(uint deviceID) -> void;

  auto power() -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  Controller* device = nullptr;
};

extern ControllerPort controllerPort;

#include "gamepad/gamepad.hpp"
