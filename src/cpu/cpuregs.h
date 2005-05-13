class CPURegFlags {
private:
  template <uint8 B> class bit {
  public:
  uint8 b;
    inline operator bool() { return (b & B); }
    inline operator = (bool i) { (i) ? b |= B : b &= ~B; return (b & B); }
    inline operator & (bool i) { if(i)b &= ~B; return (b & B); }
    inline operator | (bool i) { if(i)b |= B; return (b & B); }
    inline operator ^ (bool i) { if(i)b ^= B; return (b & B); }
  };
public:
  union {
  uint8 b;
  bit<0x80> n;
  bit<0x40> v;
  bit<0x20> m;
  bit<0x10> x;
  bit<0x08> d;
  bit<0x04> i;
  bit<0x02> z;
  bit<0x01> c;
  };

  CPURegFlags() { b = 0; }
  inline operator uint8() { return b; }
  inline unsigned operator  = (uint8 i) { b  = i; return b; }
  inline unsigned operator &= (uint8 i) { b &= i; return b; }
  inline unsigned operator |= (uint8 i) { b |= i; return b; }
  inline unsigned operator ^= (uint8 i) { b ^= i; return b; }
};

class CPUReg16 {
public:
  union {
  uint16 w;
  struct { uint8 l, h; };
  };

  CPUReg16() { w = 0; }
  inline operator uint16() { return w; }
  inline unsigned operator   = (uint16 i) { w   = i; return w; }
  inline unsigned operator  += (uint16 i) { w  += i; return w; }
  inline unsigned operator  -= (uint16 i) { w  -= i; return w; }
  inline unsigned operator  *= (uint16 i) { w  *= i; return w; }
  inline unsigned operator  /= (uint16 i) { w  /= i; return w; }
  inline unsigned operator  &= (uint16 i) { w  &= i; return w; }
  inline unsigned operator  |= (uint16 i) { w  |= i; return w; }
  inline unsigned operator  ^= (uint16 i) { w  ^= i; return w; }
  inline unsigned operator <<= (uint16 i) { w <<= i; return w; }
  inline unsigned operator >>= (uint16 i) { w >>= i; return w; }
};

class CPUReg24 {
public:
  union {
  uint16 w;
  uint32 d;
  struct { uint8 l, h, b; };
  };

  CPUReg24() { d = 0; }
  inline operator uint32() { return (d & 0xffffff); }
  inline unsigned operator   = (uint16 i) { d   = i; return d; }
  inline unsigned operator  += (uint16 i) { d  += i; return d; }
  inline unsigned operator  -= (uint16 i) { d  -= i; return d; }
  inline unsigned operator  *= (uint16 i) { d  *= i; return d; }
  inline unsigned operator  /= (uint16 i) { d  /= i; return d; }
  inline unsigned operator  &= (uint16 i) { d  &= i; return d; }
  inline unsigned operator  |= (uint16 i) { d  |= i; return d; }
  inline unsigned operator  ^= (uint16 i) { d  ^= i; return d; }
  inline unsigned operator <<= (uint16 i) { d <<= i; return d; }
  inline unsigned operator >>= (uint16 i) { d >>= i; return d; }
};

class CPURegs {
public:
CPUReg24 pc;
CPUReg16 a, x, y, s, d;
CPURegFlags p;
uint8 db;
bool  e;
  CPURegs() { db = 0; e = false; }
};
