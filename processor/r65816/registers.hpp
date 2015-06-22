struct flag_t {
  bool n{0};
  bool v{0};
  bool m{0};
  bool x{0};
  bool d{0};
  bool i{0};
  bool z{0};
  bool c{0};

  inline operator unsigned() const {
    return (n << 7) + (v << 6) + (m << 5) + (x << 4)
         + (d << 3) + (i << 2) + (z << 1) + (c << 0);
  }

  inline auto operator=(uint8 data) -> unsigned {
    n = data & 0x80; v = data & 0x40; m = data & 0x20; x = data & 0x10;
    d = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    return data;
  }
};

struct reg16_t {
  union {
    uint16 w = 0;
    struct { uint8 order_lsb2(l, h); };
  };

  inline operator unsigned() const { return w; }
  inline auto operator   = (unsigned i) -> unsigned { return w   = i; }
  inline auto operator  |= (unsigned i) -> unsigned { return w  |= i; }
  inline auto operator  ^= (unsigned i) -> unsigned { return w  ^= i; }
  inline auto operator  &= (unsigned i) -> unsigned { return w  &= i; }
  inline auto operator <<= (unsigned i) -> unsigned { return w <<= i; }
  inline auto operator >>= (unsigned i) -> unsigned { return w >>= i; }
  inline auto operator  += (unsigned i) -> unsigned { return w  += i; }
  inline auto operator  -= (unsigned i) -> unsigned { return w  -= i; }
  inline auto operator  *= (unsigned i) -> unsigned { return w  *= i; }
  inline auto operator  /= (unsigned i) -> unsigned { return w  /= i; }
  inline auto operator  %= (unsigned i) -> unsigned { return w  %= i; }
};

struct reg24_t {
  union {
    uint32 d = 0;
    struct { uint16 order_lsb2(w, wh); };
    struct { uint8  order_lsb4(l, h, b, bh); };
  };

  inline operator unsigned() const { return d; }
  inline auto operator   = (unsigned i) -> unsigned { return d = uclip<24>(i); }
  inline auto operator  |= (unsigned i) -> unsigned { return d = uclip<24>(d  | i); }
  inline auto operator  ^= (unsigned i) -> unsigned { return d = uclip<24>(d  ^ i); }
  inline auto operator  &= (unsigned i) -> unsigned { return d = uclip<24>(d  & i); }
  inline auto operator <<= (unsigned i) -> unsigned { return d = uclip<24>(d << i); }
  inline auto operator >>= (unsigned i) -> unsigned { return d = uclip<24>(d >> i); }
  inline auto operator  += (unsigned i) -> unsigned { return d = uclip<24>(d  + i); }
  inline auto operator  -= (unsigned i) -> unsigned { return d = uclip<24>(d  - i); }
  inline auto operator  *= (unsigned i) -> unsigned { return d = uclip<24>(d  * i); }
  inline auto operator  /= (unsigned i) -> unsigned { return d = uclip<24>(d  / i); }
  inline auto operator  %= (unsigned i) -> unsigned { return d = uclip<24>(d  % i); }
};

struct regs_t {
  reg24_t pc;
  reg16_t a;
  reg16_t x;
  reg16_t y;
  reg16_t z;  //pseudo-register (zero register)
  reg16_t s;
  reg16_t d;
  flag_t p;
  uint8 db{0};
  bool e{0};

  bool irq{0};       //IRQ pin (0 = low, 1 = trigger)
  bool wai{0};       //raised during wai, cleared after interrupt triggered
  uint8 mdr{0};      //memory data register
  uint16 vector{0};  //interrupt vector address
};
