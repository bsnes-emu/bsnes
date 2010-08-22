#include "dsp1emu.hpp"

class DSP1 {
public:
  void init();
  void enable();
  void power();
  void reset();

  void serialize(serializer&);

private:
  Dsp1 dsp1;
  friend class DSP1DR;
  friend class DSP1SR;
};

class DSP1DR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

class DSP1SR : public Memory {
  uint8 read(unsigned addr);
  void write(unsigned addr, uint8 data);
};

extern DSP1 dsp1;
extern DSP1DR dsp1dr;
extern DSP1SR dsp1sr;
