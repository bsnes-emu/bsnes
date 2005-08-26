class CPURegFlags {
private:
  template <uint8 B> class bit {
  public:
  uint8 _b;
    inline operator bool() { return (_b & B); }
    inline bool operator =  (bool i) { (i) ? _b |= B : _b &= ~B; return (_b & B); }
    inline bool operator &  (bool i) { if(i)_b &= ~B; return (_b & B); }
    inline bool operator &= (bool i) { if(i)_b &= ~B; return (_b & B); }
    inline bool operator |  (bool i) { if(i)_b |= B; return (_b & B); }
    inline bool operator |= (bool i) { if(i)_b |= B; return (_b & B); }
    inline bool operator ^  (bool i) { if(i)_b ^= B; return (_b & B); }
    inline bool operator ^= (bool i) { if(i)_b ^= B; return (_b & B); }
  };
public:
  union {
  uint8 _b;
  bit<0x80> n;
  bit<0x40> v;
  bit<0x20> m, p;
  bit<0x10> x, b;
  bit<0x08> d;
  bit<0x04> i;
  bit<0x02> z;
  bit<0x01> c;
  };

  CPURegFlags() { _b = 0; }
  inline operator uint8() { return _b; }
  inline unsigned operator  = (uint8 i) { _b  = i; return _b; }
  inline unsigned operator &= (uint8 i) { _b &= i; return _b; }
  inline unsigned operator |= (uint8 i) { _b |= i; return _b; }
  inline unsigned operator ^= (uint8 i) { _b ^= i; return _b; }
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
uint8 mdr; //memory data register (openbus)
bool  e;
  CPURegs() { db = 0; mdr = 0x00; e = false; }
};
