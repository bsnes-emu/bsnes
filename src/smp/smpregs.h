template<int mask>
struct SMPFlag {
  uint8 &data;

  inline operator bool() const { return data & mask; }
  inline SMPFlag& operator=(bool i) { data = (data & ~mask) | (-i & mask); return *this; }

  SMPFlag(uint8 &data_) : data(data_) {}
};

class SMPRegFlags {
public:
  uint8 data;
  SMPFlag<0x80> n;
  SMPFlag<0x40> v;
  SMPFlag<0x20> p;
  SMPFlag<0x10> b;
  SMPFlag<0x08> h;
  SMPFlag<0x04> i;
  SMPFlag<0x02> z;
  SMPFlag<0x01> c;

  inline operator unsigned() const { return data; }
  inline unsigned operator  = (unsigned i) { data  = i; return data; }
  inline unsigned operator |= (unsigned i) { data |= i; return data; }
  inline unsigned operator ^= (unsigned i) { data ^= i; return data; }
  inline unsigned operator &= (unsigned i) { data &= i; return data; }

  SMPRegFlags() : data(0), n(data), v(data), p(data), b(data), h(data), i(data), z(data), c(data) {}
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
  SMPRegs() : pc(0), ya(0), x(0), sp(0) {}
};
