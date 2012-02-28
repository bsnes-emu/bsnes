//ARM v3 (ARM6?)

struct ArmDSP : public Coprocessor {
  uint8 programROM[128 * 1024];
  uint8 programRAM[16 * 1024];
  uint8 aoRAM[32 * 1024];

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
  void opcode(uint32 data);

  void op_move_to_status_register_from_register();
  void op_move_to_register_from_status_register();
  void op_data_immediate_shift();
  void op_data_register_shift();
  void op_data_immediate();
  void op_move_immediate_offset();
  void op_move_register_offset();
  void op_move_multiple();
  void op_branch();

  //memory.cpp
  uint8 bus_iread(uint32 addr);
  void bus_iwrite(uint32 addr, uint8 data);
  template<unsigned size> uint32 bus_read(uint32 addr);
  template<unsigned size> void bus_write(uint32 addr, uint32 data);

  //disassembler.cpp
  string disassemble_opcode(uint32 pc);
  string disassemble_registers();

  //serialization.cpp
  void serialize(serializer&);
};

extern ArmDSP armdsp;
