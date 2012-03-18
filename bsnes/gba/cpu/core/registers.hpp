struct GPR {
  uint32 data;

  inline operator uint32() const { return data; }
  inline GPR& operator=(uint32 n) { data = n; return *this; }
};

struct PSR {
  struct Mode {
    enum : unsigned {
      User      = 0x10,
      FIQ       = 0x11,
      IRQ       = 0x12,
      SWI       = 0x13,
      Abort     = 0x17,
      Undefined = 0x1b,
      System    = 0x1f,
    };
  };

  bool n, z, c, v;
  bool i, f, t;
  unsigned mode;

  inline operator uint32() const {
    return (n << 31) | (z << 30) | (c << 29) | (v << 28)
         | (i << 7) | (f << 6) | (t << 5) | (mode << 0);
  }

  inline PSR& operator=(uint32 d) {
    n = d & (1 << 31);
    z = d & (1 << 30);
    c = d & (1 << 29);
    v = d & (1 << 28);
    i = d & (1 <<  7);
    f = d & (1 <<  6);
    t = d & (1 <<  5);
    mode = d & 31;
    return *this;
  }
};

struct GPRs {
  struct ID {
    enum : unsigned {
      R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, SP, LR, PC,
      R8_FIQ, R9_FIQ, R10_FIQ, R11_FIQ, R12_FIQ, R13_FIQ, R14_FIQ,
      R13_SWI, R14_SWI,
      R13_Abort, R14_Abort,
      R13_IRQ, R14_IRQ,
      R13_Undefined, R14_Undefined,
    };
  };
};

struct SPSRs {
  struct ID {
    enum : unsigned {
      FIQ, SWI, Abort, IRQ, Undefined,
    };
  };
};
