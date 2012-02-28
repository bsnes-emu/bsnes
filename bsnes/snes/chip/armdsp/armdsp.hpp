//ARM v3 (ARM6?)

struct ArmDSP : public Coprocessor {
  uint8 programROM[128 * 1024];
  uint8 programRAM[16 * 1024];

  #include "registers.hpp"

  //CPU
  uint8 r3800, r3802, r3804;
  uint8 w3800, w3802, w3804;

  //ARM
  uint32 r40000000, r40000010, r40000020, r40000024, r40000028, r4000002c;
  uint32 w40000000, w40000010, w40000020, w40000024, w40000028, w4000002c;

  //Status
  bool cputoarm;
  bool armtocpu;

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

  void op_branch();
  void op_data_immediate();
  void op_data_immediate_shift();
  void op_data_register_shift();
  void op_move_immediate_offset();
  void op_move_status_register_to_register();
  void op_move_multiple();

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
