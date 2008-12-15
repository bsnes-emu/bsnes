class SMP {
public:
  virtual void enter() = 0;

  #include "smpregs.hpp"
  regs_t regs;
  static const uint8_t iplrom[64];

  virtual uint8 ram_read(uint16 addr) = 0;
  virtual void ram_write(uint16 addr, uint8 value) = 0;
  //$f4-$f7
  virtual uint8 port_read(uint8 port) = 0;
  virtual void port_write(uint8 port, uint8 value) = 0;

  virtual void power() = 0;
  virtual void reset() = 0;

  //debugging functions
  virtual bool in_opcode();
  void disassemble_opcode(char *output);
  inline uint16 __relb(int8 offset, int op_len);

  SMP() {}
  virtual ~SMP() {}
};
