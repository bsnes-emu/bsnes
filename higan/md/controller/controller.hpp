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
  auto connect(uint device) -> void;

  auto readData() -> uint8;
  auto writeData(uint8 data) -> void;

  auto readControl() -> uint8;
  auto writeControl(uint8 data) -> void;

  auto power(uint port) -> void;
  auto unload() -> void;
  auto serialize(serializer&) -> void;

  uint port;
  uint8 control;
  Controller* controller = nullptr;
};

extern ControllerPort controllerPort1;
extern ControllerPort controllerPort2;
extern ControllerPort extensionPort;

#include "gamepad/gamepad.hpp"
