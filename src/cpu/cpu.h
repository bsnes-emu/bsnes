#include "cpuregs.h"

class CPU : public MMIO {
public:
  virtual void enter() = 0;

public:
//CPU version number
//* 1 and 2 are known
//* reported by $4210
//* affects DRAM refresh behavior
uint8 cpu_version;

//timing
  virtual uint16 vcounter() = 0;
  virtual uint16 hcounter() = 0;
  virtual uint16 hclock() = 0;
  virtual bool   interlace() = 0;
  virtual bool   interlace_field() = 0;
  virtual bool   overscan() = 0;
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
  virtual void   scanline() = 0;
  virtual void   frame() = 0;
  virtual void   power() = 0;
  virtual void   reset() = 0;

/*****
 * in opcode-based CPU emulators, the main emulation routine
 * will only be able to call the disassemble_opcode() function
 * on clean opcode edges. but with cycle-based CPU emulators,
 * the CPU may be in the middle of executing an opcode when the
 * emulator (e.g. debugger) wants to disassemble an opcode. this
 * would mean that important registers may not reflect what they
 * did at the start of the opcode (especially regs.pc), so in
 * cycle-based emulators, this function should be overridden to
 * reflect whether or not an opcode has only been partially
 * executed. if not, the debugger should abort attempts to skip,
 * disable, or disassemble the current opcode.
 *****/
  virtual bool in_opcode() { return false; }

/*****
 * opcode disassembler
 *****/
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
  OPTYPE_IADDR_PC,  //pbr:(addr)
  OPTYPE_RELB,      //relb
  OPTYPE_RELW,      //relw
};

  void   disassemble_opcode(char *output);
  uint8  dreadb(uint32 addr);
  uint16 dreadw(uint32 addr);
  uint32 dreadl(uint32 addr);
  uint32 decode(uint8 offset_type, uint32 addr);
  uint8  opcode_length();

  CPU();
  virtual ~CPU();
};
