class CPURegFlags {
private:
  template<uint mask> class bit {
  public:
  uint data;
    inline operator bool() { return bool(data & mask); }
    inline bool operator  = (const bool i) { (i) ? data |= mask : data &= ~mask; return bool(data & mask); }
    inline bool operator |= (const bool i) { if(i)data |= mask; return bool(data & mask); }
    inline bool operator ^= (const bool i) { if(i)data ^= mask; return bool(data & mask); }
    inline bool operator &= (const bool i) { if(i)data &= mask; return bool(data & mask); }
  };

public:
union {
  uint8 data;
  bit<0x80> n;
  bit<0x40> v;
  bit<0x20> m, p;
  bit<0x10> x, b;
  bit<0x08> d;
  bit<0x04> i;
  bit<0x02> z;
  bit<0x01> c;
};

  inline operator unsigned() const { return data; }
  inline unsigned operator  = (const uint8 i) { data  = i; return data; }
  inline unsigned operator |= (const uint8 i) { data |= i; return data; }
  inline unsigned operator ^= (const uint8 i) { data ^= i; return data; }
  inline unsigned operator &= (const uint8 i) { data &= i; return data; }

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
uint8  db;
uint8  mdr;
bool   e;
bool   acc_8b, idx_8b;
  CPURegs() : db(0), mdr(0x00), e(false), acc_8b(true), idx_8b(true) {}
};
