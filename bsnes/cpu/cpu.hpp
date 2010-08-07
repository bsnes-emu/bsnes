#if defined(DEBUGGER)
  #include "cpu-debugger.hpp"
#endif

class CPU : public Processor, public PPUCounter, public MMIO {
public:
  //synchronization
  array<Processor*> coprocessors;
  alwaysinline void step(unsigned clocks);
  alwaysinline void synchronize_smp();
  alwaysinline void synchronize_ppu();
  alwaysinline void synchronize_coprocessor();

  static void Enter();
  virtual void enter() = 0;

  //CPU version number
  //* 1 and 2 are known
  //* reported by $4210
  //* affects timing (DRAM refresh, HDMA init, etc)
  uint8 cpu_version;

  virtual uint8 pio() = 0;
  virtual bool joylatch() = 0;
  virtual uint8 port_read(uint8 port) = 0;
  virtual void port_write(uint8 port, uint8 value) = 0;

  virtual void scanline() = 0;
  virtual void power();
  virtual void reset();

  virtual void serialize(serializer&);
  CPU();
  virtual ~CPU();
};
