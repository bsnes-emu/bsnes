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

struct Controller : Processor {
  enum : bool { Port1 = 0, Port2 = 1 };
  const bool port;

  bool iobit();
  void iobit(bool data);
  virtual uint2 data() { return 0; }
  virtual void latch(bool data) {}
  Controller(bool port);
};

struct Controllers {
  Controller *port1;
  Controller *port2;

  void connect(bool port, Input::Device id);
  Controllers();
  ~Controllers();
};

extern Controllers controllers;

#include "gamepad/gamepad.hpp"
#include "multitap/multitap.hpp"
#include "mouse/mouse.hpp"
#include "superscope/superscope.hpp"
#include "justifier/justifier.hpp"
