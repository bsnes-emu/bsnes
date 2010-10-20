#include "../snes_spc/SNES_SPC.h"

class SMP : public Processor {
public:
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();
  alwaysinline void synchronize_dsp();

  uint8 port_read(uint8 port);
  void port_write(uint8 port, uint8 data);

  void run();
  void power();
  void reset();

  void serialize(serializer&);
  SMP();
  ~SMP();

private:
  SNES_SPC snes_spc;
  unsigned snes_spc_time;
  int16 samplebuffer[8192];
  static const uint8 iplrom[64];

  friend class SMPcore;
};

extern SMP smp;
