struct Reg16 {
  union {
    uint16 w;
    struct { uint8 order_lsb2(l, h); };
  };

  inline operator unsigned() const { return w; }
  inline unsigned operator=(const unsigned i) { return w = i; }

  Reg16() : w(0) {}
};

template<int bit> struct RegFlag8 {
  uint8 data;

  inline operator bool() const { return data & bit; }
  inline bool operator=(const bool i) { i ? data |= bit : data &= ~bit; return i; }
};

template<int bit> struct RegFlag16 {
  uint16 data;

  inline operator bool() const { return data & bit; }
  inline bool operator=(const bool i) { i ? data |= bit : data &= ~bit; return i; }
};

struct SFR {
  union {
    uint16 w;
    struct { uint8 order_lsb2(l, h); };
    RegFlag16<0x0002> z;    //zero flag
    RegFlag16<0x0004> c;    //carry flag
    RegFlag16<0x0008> s;    //sign flag
    RegFlag16<0x0010> v;    //overflow flag
    RegFlag16<0x0020> g;    //go flag
    RegFlag16<0x0040> r;    //ROM read using r14 flag
    RegFlag16<0x0100> alt1; //alternate instruction 1 flag
    RegFlag16<0x0200> alt2; //alternate instruction 2 flag
    RegFlag16<0x0400> il;   //immediate lower 8-bit flag
    RegFlag16<0x0800> ih;   //immediate upper 8-bit flag
    RegFlag16<0x1000> b;    //WITH instruction flag
    RegFlag16<0x8000> irq;  //interrupt flag
  };

  inline operator unsigned() const { return w & 0x9f7e; } //invalid flag bits always return 0 when read
  inline unsigned operator=(const unsigned i) { return w = i & 0x9f7e; }

  SFR() : w(0) {}
};

struct RAMBR {
  union {
    uint8 b;
    RegFlag8<0x01> bank;
  };

  inline operator unsigned() const { return b & 0x01; }
  inline unsigned operator=(const unsigned i) { return b = i & 0x01; }

  RAMBR() : b(0) {}
};

struct CBR {
  uint16 w;

  inline operator unsigned() const { return w & 0xfff0; }
  inline unsigned operator=(const unsigned i) { return w = i & 0xfff0; }

  CBR() : w(0) {}
};

struct SCMR {
  union {
    uint8 b;
    RegFlag8<0x01> md0; //color mode low
    RegFlag8<0x02> md1; //color mode high
    RegFlag8<0x04> ht0; //height low
    RegFlag8<0x08> ran; //ram enable
    RegFlag8<0x10> ron; //rom enable
    RegFlag8<0x20> ht1; //height high
  };

  inline operator unsigned() const { return b; }
  inline unsigned operator=(const unsigned i) { return b = i; }

  SCMR() : b(0) {}
};

struct BRAMR {
  union {
    uint8 b;
    RegFlag8<0x01> flag;
  };

  inline operator unsigned() const { return b; }
  inline unsigned operator=(const unsigned i) { return b = i; }

  BRAMR() : b(0) {}
};

struct CFGR {
  union {
    uint8 b;
    RegFlag8<0x20> ms0; //multiplier speed selection
    RegFlag8<0x80> irq; //irq mask flag
  };

  inline operator unsigned() const { return b; }
  inline unsigned operator=(const unsigned i) { return b = i; }

  CFGR() : b(0) {}
};

struct CLSR {
  union {
    uint8 b;
    RegFlag8<0x01> flag;
  };

  inline operator unsigned() const { return b; }
  inline unsigned operator=(const unsigned i) { return b = i; }

  CLSR() : b(0) {}
};

struct POR {
  union {
    uint8 b;
    RegFlag8<0x01> transparent; //transparent flag
    RegFlag8<0x02> dither;      //dither flag
    RegFlag8<0x04> highnibble;  //high nibble flag
    RegFlag8<0x08> freezehigh;  //freeze high flag
    RegFlag8<0x10> obj;         //OBJ flag
  };

  inline operator unsigned() const { return b; }
  inline unsigned operator=(const unsigned i) { return b = i; }

  POR() : b(0) {}
};

struct Regs {
  Reg16  r0;    //default source/destination register
  Reg16  r1;    //pixel plot X position register
  Reg16  r2;    //pixel plot Y position register
  Reg16  r3;
  Reg16  r4;    //lower 16-bit result of lmult
  Reg16  r5;
  Reg16  r6;    //multiplier for fmult and lmult
  Reg16  r7;    //fixed point texel X position for merge
  Reg16  r8;    //fixed point texel Y position for merge
  Reg16  r9;
  Reg16  r10;
  Reg16  r11;   //return address set by link
  Reg16  r12;   //loop counter
  Reg16  r13;   //loop point address
  Reg16  r14;   //rom address for getb, getbh, getbl, getbs
  Reg16  r15;   //program counter

  SFR    sfr;   //status/flag register
  uint8  pbr;   //program bank register
  uint8  rombr; //rom bank register
  RAMBR  rambr; //ram bank register
  CBR    cbr;   //cache base register
  uint8  scbr;  //screen base register
  SCMR   scmr;  //screen mode register
  BRAMR  bramr; //backup ram register
  uint8  vcr;   //version code register
  CFGR   cfgr;  //config register
  CLSR   clsr;  //clock select register

  uint8  colr;  //color register
  POR    por;   //plot option register
} regs;
