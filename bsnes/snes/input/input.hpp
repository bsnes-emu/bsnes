class Input {
public:
  enum class Device : unsigned {
    None,
    Joypad,
    Multitap,
    Mouse,
    SuperScope,
    Justifier,
    Justifiers,
  };

  enum class JoypadID : unsigned {
    B  =  0, Y    =  1, Select =  2, Start =  3,
    Up =  4, Down =  5, Left   =  6, Right =  7,
    A  =  8, X    =  9, L      = 10, R     = 11,
  };

  enum class MouseID : unsigned {
    X = 0, Y = 1, Left = 2, Right = 3,
  };

  enum class SuperScopeID : unsigned {
    X = 0, Y = 1, Trigger = 2, Cursor = 3, Turbo = 4, Pause = 5,
  };

  enum class JustifierID : unsigned {
    X = 0, Y = 1, Trigger = 2, Start = 3,
  };

  uint8 port_read(bool port);
  void port_set_device_(bool port, Device device);
  void init();
  void poll();
  void update();

  //light guns (Super Scope, Justifier(s)) strobe IOBit whenever the CRT
  //beam cannon is detected. this needs to be tested at the cycle level
  //(hence inlining here for speed) to avoid 'dead space' during DRAM refresh.
  //iobit is updated during port_set_device(),
  //latchx, latchy are updated during update() (once per frame)
  alwaysinline void tick() {
    //only test if Super Scope or Justifier is connected
    if(iobit && cpu.vcounter() == latchy && cpu.hcounter() == latchx) {
      ppu.latch_counters();
    }
  }

private:
  bool iobit;
  int16_t latchx, latchy;

  struct port_t {
    Device device;
    unsigned counter0;  //read counters
    unsigned counter1;

    struct superscope_t {
      int x, y;

      bool trigger;
      bool cursor;
      bool turbo;
      bool pause;
      bool offscreen;

      bool turbolock;
      bool triggerlock;
      bool pauselock;
    } superscope;

    struct justifier_t {
      bool active;

      int x1, x2;
      int y1, y2;

      bool trigger1, trigger2;
      bool start1, start2;
    } justifier;
  } port[2];

  friend class System;
  friend class Video;
  friend class CPU;
};

extern Input input1;
