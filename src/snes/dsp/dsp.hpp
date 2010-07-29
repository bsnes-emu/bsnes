#if defined(DEBUGGER)
  #include "dsp-debugger.hpp"
#endif

class DSP : public Processor {
public:
  //synchronization
  alwaysinline void create();
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_smp();
  virtual void enter() = 0;

  virtual uint8 read(uint8 addr) = 0;
  virtual void write(uint8 addr, uint8 data) = 0;

  virtual void power();
  virtual void reset();

  virtual void serialize(serializer&);
};
