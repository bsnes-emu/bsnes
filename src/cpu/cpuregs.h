class CPURegFlags {
public:
union {
  uint8 data;
  struct {
    uint8 order_msb8(n:1, v:1, m:1, x:1, d:1, i:1, z:1, c:1);
  };
};

  inline operator unsigned() const { return data; }
  template<typename T> inline unsigned operator  = (const T i) { data  = i; return data; }
  template<typename T> inline unsigned operator |= (const T i) { data |= i; return data; }
  template<typename T> inline unsigned operator ^= (const T i) { data ^= i; return data; }
  template<typename T> inline unsigned operator &= (const T i) { data &= i; return data; }

  CPURegFlags() : data(0) {}
};

class CPUReg16 {
public:
union {
  uint16 w;
  struct { uint8 order_lsb2(l, h); };
};

  inline operator unsigned() const { return w; }
  template<typename T> inline unsigned operator   = (const T i) { w   = i; return w; }
  template<typename T> inline unsigned operator  |= (const T i) { w  |= i; return w; }
  template<typename T> inline unsigned operator  ^= (const T i) { w  ^= i; return w; }
  template<typename T> inline unsigned operator  &= (const T i) { w  &= i; return w; }
  template<typename T> inline unsigned operator <<= (const T i) { w <<= i; return w; }
  template<typename T> inline unsigned operator >>= (const T i) { w >>= i; return w; }
  template<typename T> inline unsigned operator  += (const T i) { w  += i; return w; }
  template<typename T> inline unsigned operator  -= (const T i) { w  -= i; return w; }
  template<typename T> inline unsigned operator  *= (const T i) { w  *= i; return w; }
  template<typename T> inline unsigned operator  /= (const T i) { w  /= i; return w; }
  template<typename T> inline unsigned operator  %= (const T i) { w  %= i; return w; }

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
  template<typename T> inline unsigned operator   = (const T i) { d = uclip<24>(i);      return d; }
  template<typename T> inline unsigned operator  |= (const T i) { d = uclip<24>(d  | i); return d; }
  template<typename T> inline unsigned operator  ^= (const T i) { d = uclip<24>(d  ^ i); return d; }
  template<typename T> inline unsigned operator  &= (const T i) { d = uclip<24>(d  & i); return d; }
  template<typename T> inline unsigned operator <<= (const T i) { d = uclip<24>(d << i); return d; }
  template<typename T> inline unsigned operator >>= (const T i) { d = uclip<24>(d >> i); return d; }
  template<typename T> inline unsigned operator  += (const T i) { d = uclip<24>(d  + i); return d; }
  template<typename T> inline unsigned operator  -= (const T i) { d = uclip<24>(d  - i); return d; }
  template<typename T> inline unsigned operator  *= (const T i) { d = uclip<24>(d  * i); return d; }
  template<typename T> inline unsigned operator  /= (const T i) { d = uclip<24>(d  / i); return d; }
  template<typename T> inline unsigned operator  %= (const T i) { d = uclip<24>(d  % i); return d; }

  CPUReg24() : d(0) {}
};

class CPURegs {
public:
CPUReg24 pc;
CPUReg16 a, x, y, s, d;
CPURegFlags p;
uint8 db;
uint8 mdr;
bool  e;
  CPURegs() : db(0), mdr(0x00), e(false) {}
};
