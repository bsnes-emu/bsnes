struct Flags {
  bool n{0};
  bool v{0};
  bool m{0};
  bool x{0};
  bool d{0};
  bool i{0};
  bool z{0};
  bool c{0};

  inline operator uint() const {
    return (n << 7) + (v << 6) + (m << 5) + (x << 4)
         + (d << 3) + (i << 2) + (z << 1) + (c << 0);
  }

  inline auto operator=(uint8 data) -> uint {
    n = data & 0x80; v = data & 0x40; m = data & 0x20; x = data & 0x10;
    d = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    return data;
  }
};

struct reg16 {
  union {
    uint16_t w = 0;
    struct { uint8_t order_lsb2(l, h); };
  };

  inline operator uint() const { return w; }
  inline auto operator  =(uint i) -> uint { return w   = i; }
  inline auto operator |=(uint i) -> uint { return w  |= i; }
  inline auto operator ^=(uint i) -> uint { return w  ^= i; }
  inline auto operator &=(uint i) -> uint { return w  &= i; }
  inline auto operator<<=(uint i) -> uint { return w <<= i; }
  inline auto operator>>=(uint i) -> uint { return w >>= i; }
  inline auto operator +=(uint i) -> uint { return w  += i; }
  inline auto operator -=(uint i) -> uint { return w  -= i; }
  inline auto operator *=(uint i) -> uint { return w  *= i; }
  inline auto operator /=(uint i) -> uint { return w  /= i; }
  inline auto operator %=(uint i) -> uint { return w  %= i; }
};

struct reg24 {
  union {
    uint32_t d = 0;
    struct { uint16_t order_lsb2(w, wh); };
    struct { uint8_t  order_lsb4(l, h, b, bh); };
  };

  inline operator uint() const { return d; }
  inline auto operator  =(uint i) -> uint { return d = uclip<24>(i); }
  inline auto operator |=(uint i) -> uint { return d = uclip<24>(d  | i); }
  inline auto operator ^=(uint i) -> uint { return d = uclip<24>(d  ^ i); }
  inline auto operator &=(uint i) -> uint { return d = uclip<24>(d  & i); }
  inline auto operator<<=(uint i) -> uint { return d = uclip<24>(d << i); }
  inline auto operator>>=(uint i) -> uint { return d = uclip<24>(d >> i); }
  inline auto operator +=(uint i) -> uint { return d = uclip<24>(d  + i); }
  inline auto operator -=(uint i) -> uint { return d = uclip<24>(d  - i); }
  inline auto operator *=(uint i) -> uint { return d = uclip<24>(d  * i); }
  inline auto operator /=(uint i) -> uint { return d = uclip<24>(d  / i); }
  inline auto operator %=(uint i) -> uint { return d = uclip<24>(d  % i); }
};

struct Registers {
  reg24 pc;
  reg16 a;
  reg16 x;
  reg16 y;
  reg16 z;  //pseudo-register (zero register)
  reg16 s;
  reg16 d;
  Flags p;
  uint8 db{0};
  bool e{0};

  bool irq{0};       //IRQ pin (0 = low, 1 = trigger)
  bool wai{0};       //raised during wai, cleared after interrupt triggered
  uint8 mdr{0};      //memory data register
  uint16 vector{0};  //interrupt vector address
};
