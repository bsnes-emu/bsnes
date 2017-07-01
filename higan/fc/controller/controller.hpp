//Famicom controller port pinout:
//  ____
// |    \
// |(7)  \
// |(2)(1)|
// |(3)(5)|
// |(4)(6)|
// |______|
//
// pin  name   port1           port2
//  1:  +5v
//  2:  clock  $4016 read      $4016.d0 write
//  3:  latch  $4016.d0 write  $4016.d0 write
//  4:  data0  $4016.d0 read   $4017.d0 read
//  5:  data3  $4016.d3 read   $4017.d3 read
//  6:  data4  $4016.d4 read   $4017.d4 read
//  7:  gnd

struct Controller : Thread {
  Controller(uint port);
  virtual ~Controller();
  static auto Enter() -> void;

  virtual auto main() -> void;
  virtual auto data() -> uint3 { return 0; }
  virtual auto latch(bool data) -> void {}

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

#include "gamepad/gamepad.hpp"
