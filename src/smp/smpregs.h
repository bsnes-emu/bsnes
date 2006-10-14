class SMPRegFlags {
public:
union {
  uint8 data;
  struct {
    uint8 order_msb8(n:1, v:1, p:1, b:1, h:1, i:1, z:1, c:1);
  };
};

  inline operator unsigned() const { return data; }
  template<typename T> inline unsigned operator  = (const T i) { data  = i; return data; }
  template<typename T> inline unsigned operator |= (const T i) { data |= i; return data; }
  template<typename T> inline unsigned operator ^= (const T i) { data ^= i; return data; }
  template<typename T> inline unsigned operator &= (const T i) { data &= i; return data; }

  SMPRegFlags() : data(0) {}
};

class SMPRegs {
public:
uint16 pc;
union {
  uint16 ya;
  struct { uint8 order_lsb2(a, y); };
};
uint8 x, sp;
SMPRegFlags p;
};
