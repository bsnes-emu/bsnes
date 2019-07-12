#include "SPC_DSP.h"

struct DSP {
  shared_pointer<Emulator::Stream> stream;
  uint8 apuram[64 * 1024] = {};

  void main();
  uint8 read(uint8 addr);
  void write(uint8 addr, uint8 data);

  bool load();
  void power(bool reset);
  bool mute();

  void serialize(serializer&);

  int64 clock = 0;

private:
  SPC_DSP spc_dsp;
  int16_t samplebuffer[8192];
};

extern DSP dsp;
