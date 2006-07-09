class APURegFlags {
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
  bit<0x20> p;
  bit<0x10> b;
  bit<0x08> h;
  bit<0x04> i;
  bit<0x02> z;
  bit<0x01> c;
};

  APURegFlags() { data = 0; }
  inline operator unsigned() { return data; }
  inline unsigned operator  = (const uint8 i) { data  = i; return data; }
  inline unsigned operator |= (const uint8 i) { data |= i; return data; }
  inline unsigned operator ^= (const uint8 i) { data ^= i; return data; }
  inline unsigned operator &= (const uint8 i) { data &= i; return data; }
};

class APURegs {
public:
uint16 pc;
union {
  uint16 ya;
  struct { uint8 order_lsb2(a, y); };
};
uint8 x, sp;
APURegFlags p;
};
