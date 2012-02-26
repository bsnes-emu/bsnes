//Exceptions:
//00000000 = reset
//00000004 = undefined instruction
//00000008 = software interrupt
//0000000c = prefetch abort
//00000010 = data abort
//00000018 = IRQ (interrupt)
//0000001c = FIQ (fast interrupt)

struct CPSR {
  bool n;
  bool z;
  bool c;
  bool v;
  bool i;
  bool f;
  uint5 m;

  operator unsigned() const {
    return (n << 31) | (z << 30) | (c << 29) | (v << 28) | (i << 7) | (f << 6) | (m << 0);
  }

  CPSR& operator=(uint32 data) {
    n = data & 0x80000000;
    z = data & 0x40000000;
    c = data & 0x20000000;
    v = data & 0x10000000;
    i = data & 0x00000080;
    f = data & 0x00000040;
    m = data & 0x0000001f;
    return *this;
  }
} cpsr;

//r13 = SP (stack pointer)
//r14 = LR (link register)
//r15 = PC (program counter)
uint32 r[16];

uint32 instruction;
uint32 prefetch;
