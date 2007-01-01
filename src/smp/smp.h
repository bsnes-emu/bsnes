#include "smpregs.h"

class SMP {
public:
  virtual void enter() = 0;

public:
SMPRegs regs;
uint8 spcram[65536];
static const uint8 iplrom[64];
  enum {
    FLAG_N = 0x80, FLAG_V = 0x40, FLAG_P = 0x20, FLAG_B = 0x10,
    FLAG_H = 0x08, FLAG_I = 0x04, FLAG_Z = 0x02, FLAG_C = 0x01
  };

  virtual uint8  ram_read (uint16 addr) = 0;
  virtual void   ram_write(uint16 addr, uint8 value) = 0;
//$f4-$f7
  virtual uint8  port_read (uint8 port) = 0;
  virtual void   port_write(uint8 port, uint8 value) = 0;

  virtual uint8 *get_spcram_handle() { return spcram; }
  virtual void   power() = 0;
  virtual void   reset() = 0;

//debugging functions
  virtual bool in_opcode();
  void disassemble_opcode(char *output);
  inline uint16 __relb(int8 offset, int op_len);

  SMP() { memset(spcram, 0, 65536); }
  virtual ~SMP() {}
};
