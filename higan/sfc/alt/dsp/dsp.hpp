#include "SPC_DSP.h"

struct DSP : Thread {
  enum : bool { Threaded = false };

  DSP();

  alwaysinline auto step(uint clocks) -> void;
  alwaysinline auto synchronizeSMP() -> void;

  auto mute() -> bool;
  auto read(uint8 addr) -> uint8;
  auto write(uint8 addr, uint8 data) -> void;

  auto enter() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto channel_enable(uint channel, bool enable) -> void;

  auto serialize(serializer&) -> void;

private:
  SPC_DSP spc_dsp;
  int16 samplebuffer[8192];
  bool channel_enabled[8];
};

extern DSP dsp;
