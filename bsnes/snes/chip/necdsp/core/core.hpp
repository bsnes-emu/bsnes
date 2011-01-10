class uPDcore {
public:
  #include "registers.hpp"

  uint24 *programROM;
  uint16 *dataROM;
  uint16 *dataRAM;

  unsigned programROMSize;
  unsigned dataROMSize;
  unsigned dataRAMSize;

  void exec();
  void exec_op(uint24 opcode);
  void exec_rt(uint24 opcode);
  void exec_jp(uint24 opcode);
  void exec_ld(uint24 opcode);
  void stack_push();
  void stack_pull();

  string disassemble(uint14 ip);

  void serialize(serializer&);
  uPDcore(unsigned pcbits, unsigned rpbits, unsigned dpbits);
  ~uPDcore();
};
