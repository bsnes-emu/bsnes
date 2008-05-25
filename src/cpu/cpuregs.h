template<int mask>
struct CPUFlag {
  uint8 &data;

  inline operator bool() const { return data & mask; }
  inline CPUFlag& operator=(bool i) { data = (data & ~mask) | (-i & mask); return *this; }

  CPUFlag(uint8 &data_) : data(data_) {}
};

class CPURegFlags {
public:
  uint8 data;
  CPUFlag<0x80> n;
  CPUFlag<0x40> v;
  CPUFlag<0x20> m;
  CPUFlag<0x10> x;
  CPUFlag<0x08> d;
  CPUFlag<0x04> i;
  CPUFlag<0x02> z;
  CPUFlag<0x01> c;

  inline operator unsigned() const { return data; }
  inline unsigned operator  = (unsigned i) { data  = i; return data; }
  inline unsigned operator |= (unsigned i) { data |= i; return data; }
  inline unsigned operator ^= (unsigned i) { data ^= i; return data; }
  inline unsigned operator &= (unsigned i) { data &= i; return data; }

  CPURegFlags() : data(0), n(data), v(data), m(data), x(data), d(data), i(data), z(data), c(data) {}
};

class CPUReg16 {
public:
  union {
    uint16 w;
    struct { uint8 order_lsb2(l, h); };
  };

  inline operator unsigned() const { return w; }
  inline unsigned operator   = (unsigned i) { w   = i; return w; }
  inline unsigned operator  |= (unsigned i) { w  |= i; return w; }
  inline unsigned operator  ^= (unsigned i) { w  ^= i; return w; }
  inline unsigned operator  &= (unsigned i) { w  &= i; return w; }
  inline unsigned operator <<= (unsigned i) { w <<= i; return w; }
  inline unsigned operator >>= (unsigned i) { w >>= i; return w; }
  inline unsigned operator  += (unsigned i) { w  += i; return w; }
  inline unsigned operator  -= (unsigned i) { w  -= i; return w; }
  inline unsigned operator  *= (unsigned i) { w  *= i; return w; }
  inline unsigned operator  /= (unsigned i) { w  /= i; return w; }
  inline unsigned operator  %= (unsigned i) { w  %= i; return w; }

  CPUReg16() : w(0) {}
};

class CPUReg24 {
public:
  union {
    uint32 d;
    struct { uint16 order_lsb2(w, wh); };
    struct { uint8  order_lsb4(l, h, b, bh); };
  };

  inline operator unsigned() const { return d; }
  inline unsigned operator   = (unsigned i) { d = uclip<24>(i);      return d; }
  inline unsigned operator  |= (unsigned i) { d = uclip<24>(d  | i); return d; }
  inline unsigned operator  ^= (unsigned i) { d = uclip<24>(d  ^ i); return d; }
  inline unsigned operator  &= (unsigned i) { d = uclip<24>(d  & i); return d; }
  inline unsigned operator <<= (unsigned i) { d = uclip<24>(d << i); return d; }
  inline unsigned operator >>= (unsigned i) { d = uclip<24>(d >> i); return d; }
  inline unsigned operator  += (unsigned i) { d = uclip<24>(d  + i); return d; }
  inline unsigned operator  -= (unsigned i) { d = uclip<24>(d  - i); return d; }
  inline unsigned operator  *= (unsigned i) { d = uclip<24>(d  * i); return d; }
  inline unsigned operator  /= (unsigned i) { d = uclip<24>(d  / i); return d; }
  inline unsigned operator  %= (unsigned i) { d = uclip<24>(d  % i); return d; }

  CPUReg24() : d(0) {}
};

class CPURegs {
public:
  CPUReg24 pc;
  CPUReg16 a, x, y, s, d;
  CPURegFlags p;
  uint8 db;
  uint8 mdr;
  bool e;
  CPURegs() : db(0), mdr(0), e(false) {}
};
