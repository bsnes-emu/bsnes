//ARMv3 (ARM60)

struct ArmDSP : Processor::ARM, Coprocessor {
  uint8* programROM;
  uint8* dataROM;
  uint8* programRAM;

  #include "registers.hpp"

  static void Enter();
  void enter();

  void step(unsigned clocks) override;
  void bus_idle() override;
  uint32 bus_read(unsigned mode, uint32 addr) override;
  void bus_write(unsigned mode, uint32 addr, uint32 word) override;

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  void init();
  void load();
  void unload();
  void power();
  void reset();
  void arm_reset();

  nall::vector<uint8> firmware();
  void serialize(serializer&);

  ArmDSP();
  ~ArmDSP();
};

extern ArmDSP armdsp;
