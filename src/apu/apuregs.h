class APURegFlags {
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
  bit<0x20> p;
  bit<0x10> b;
  bit<0x08> h;
  bit<0x04> i;
  bit<0x02> z;
  bit<0x01> c;
  };

  APURegFlags() { _b = 0; }
  inline operator uint8() { return _b; }
  inline unsigned operator  = (uint8 i) { _b  = i; return _b; }
  inline unsigned operator &= (uint8 i) { _b &= i; return _b; }
  inline unsigned operator |= (uint8 i) { _b |= i; return _b; }
  inline unsigned operator ^= (uint8 i) { _b ^= i; return _b; }
};

class APURegs {
public:
uint16 pc;
union {
  uint16 ya;
//not endian-safe
  struct {
    uint8 a, y;
  };
};
uint8 x, sp;
APURegFlags p;
};
