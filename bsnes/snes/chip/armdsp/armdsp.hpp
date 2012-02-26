//ARM v3 (ARM6?)

struct ArmDSP : public Coprocessor {
  uint8 programROM[128 * 1024];

  #include "registers.hpp"

  static void Enter();
  void enter();

  void init();
  void load();
  void unload();
  void power();
  void reset();

  uint8 mmio_read(unsigned addr);
  void mmio_write(unsigned addr, uint8 data);

  //opcodes.cpp
  bool condition();
  void opcode(uint32 &rd, uint32 &rn, uint32 shifter);
  void flags(uint32 rd);

  void op_branch();
  void op_data_immediate();
  void op_move_immediate_offset();

  //memory.cpp
  uint32 bus_read(uint32 addr);
  void bus_write(uint32 addr, uint32 data);

  //disassembler.cpp
  string disassemble_opcode(uint32 pc);
  string disassemble_registers();

  //serialization.cpp
  void serialize(serializer&);
};

extern ArmDSP armdsp;
