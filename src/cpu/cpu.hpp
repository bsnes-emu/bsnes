class CPU : public MMIO {
public:
  virtual void enter() = 0;

  //CPU version number
  //* 1 and 2 are known
  //* reported by $4210
  //* affects timing (DRAM refresh, HDMA init, etc)
  uint8 cpu_version;

  virtual uint8 pio() = 0;
  virtual bool joylatch() = 0;
  virtual uint8 port_read(uint8 port) = 0;
  virtual void port_write(uint8 port, uint8 value) = 0;

  #include "cpuregs.hpp"
  regs_t regs;

  virtual void scanline() = 0;
  virtual void power();
  virtual void reset();

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
