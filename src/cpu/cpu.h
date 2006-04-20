#include "cpuregs.h"

class CPU : public MMIO {
public:
//CPU version number
//* 1 and 2 are known
//* reported by $4210
//* affects DRAM refresh behavior
uint8 cpu_version;

//timing
  virtual uint16 vcounter() = 0;
  virtual uint16 hcounter() = 0;
  virtual uint16 hcycles() = 0;
  virtual bool   overscan() = 0;
  virtual bool   interlace() = 0;
  virtual bool   interlace_field() = 0;
  virtual uint16 region_scanlines() = 0;
  virtual void   set_interlace(bool r) = 0;
  virtual void   set_overscan (bool r) = 0;

CPURegs regs;
  virtual uint8 port_read (uint8 port) = 0;
  virtual void  port_write(uint8 port, uint8 value) = 0;

  enum {
    FLAG_N = 0x80, FLAG_V = 0x40,
    FLAG_M = 0x20, FLAG_X = 0x10,
    FLAG_D = 0x08, FLAG_I = 0x04,
    FLAG_Z = 0x02, FLAG_C = 0x01
  };
  virtual uint8  pio_status() = 0;
  virtual void   run() = 0;
  virtual uint32 cycles_executed() = 0;
  virtual void   scanline() = 0;
  virtual void   frame() = 0;
  virtual void   power() = 0;
  virtual void   reset() = 0;

//opcode disassembler
enum {
  OPTYPE_DP = 0,    //dp
  OPTYPE_DPX,       //dp,x
  OPTYPE_DPY,       //dp,y
  OPTYPE_IDP,       //(dp)
  OPTYPE_IDPX,      //(dp,x)
  OPTYPE_IDPY,      //(dp),y
  OPTYPE_ILDP,      //[dp]
  OPTYPE_ILDPY,     //[dp],y
  OPTYPE_ADDR,      //addr
  OPTYPE_ADDRX,     //addr,x
  OPTYPE_ADDRY,     //addr,y
  OPTYPE_IADDRX,    //(addr,x)
  OPTYPE_ILADDR,    //[addr]
  OPTYPE_LONG,      //long
  OPTYPE_LONGX,     //long, x
  OPTYPE_SR,        //sr,s
  OPTYPE_ISRY,      //(sr,s),y
  OPTYPE_ADDR_PC,   //pbr:addr
  OPTYPE_IADDR_PC   //pbr:(addr)
};
//see dcpu.cpp for notes on this function
  virtual bool in_opcode();

  void   disassemble_opcode(char *output);
  uint32 resolve_offset(uint8 offset_type, uint32 addr);
  uint8  opcode_length();
  uint16 __relb(int8 offset);
  uint16 __relw(int16 offset);

  CPU();
  virtual ~CPU() {}
};
