// SNES controller port pinout:
//  -------------------------------
// | (1) (2) (3) (4) | (5) (6) (7) )
//  -------------------------------
// pin    name     port1                            port2
//  1:    +5v
//  2:    clock    $4016 read                       $4017 read
//  3:    latch    $4016.d0 write                   $4016.d0 write
//  4:    data1    $4016.d0 read                    $4017.d0 read
//  5:    data2    $4016.d1 read                    $4017.d1 read
//  6:    iobit    $4201.d6 write; $4213.d6 read    $4201.d7 write; $4213.d7 read
//  7:    gnd

struct Controller : Thread {
  enum : bool { Port1 = 0, Port2 = 1 };

  Controller(bool port);
  virtual ~Controller();
  static auto Enter() -> void;

  virtual auto main() -> void;
  auto iobit() -> bool;
  auto iobit(bool data) -> void;
  virtual auto data() -> uint2 { return 0; }
  virtual auto latch(bool data) -> void {}

  const bool port;
};

#include "gamepad/gamepad.hpp"
#include "mouse/mouse.hpp"
#include "super-multitap/super-multitap.hpp"
#include "super-scope/super-scope.hpp"
#include "justifier/justifier.hpp"
