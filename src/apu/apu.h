#define _APU_IPLROM
#include "iplrom.h"
#include "apuregs.h"

class APU {
public:
APURegs regs;
  enum {
    FLAG_N = 0x80, FLAG_V = 0x40,
    FLAG_P = 0x20, FLAG_B = 0x10,
    FLAG_H = 0x08, FLAG_I = 0x04,
    FLAG_Z = 0x02, FLAG_C = 0x01
  };

  virtual uint8  spcram_read (uint16 addr) = 0;
  virtual void   spcram_write(uint16 addr, uint8 value) = 0;
//$f4-$f7
  virtual uint8  port_read (uint8 port) = 0;
  virtual void   port_write(uint8 port, uint8 value) = 0;

  virtual void   run() = 0;
  virtual uint32 cycles_executed() = 0;
  virtual void   power() = 0;
  virtual void   reset() = 0;

  void disassemble_opcode(char *output);
  inline uint16 __relb(int8 offset, int op_len);
};
