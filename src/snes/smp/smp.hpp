#if defined(DEBUGGER)
  #include "smp-debugger.hpp"
#endif

class SMP : public Processor {
public:
  //synchronization
  alwaysinline void create();
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_cpu();
  alwaysinline void synchronize_dsp();

  virtual void enter() = 0;

  static const uint8 iplrom[64];

  virtual uint8 ram_read(uint16 addr) = 0;
  virtual void ram_write(uint16 addr, uint8 value) = 0;

  //$f4-$f7
  virtual uint8 port_read(uint8 port) = 0;
  virtual void port_write(uint8 port, uint8 value) = 0;

  virtual void power();
  virtual void reset();

  virtual void serialize(serializer&);
  SMP() {}
  virtual ~SMP() {}
};
