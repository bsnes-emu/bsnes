struct Flag {
  inline operator uint() const {
    return (n << 7) | (v << 6) | (p << 5) | (b << 4)
         | (h << 3) | (i << 2) | (z << 1) | (c << 0);
  }

  inline auto operator=(uint8 data) -> uint {
    n = data & 0x80; v = data & 0x40; p = data & 0x20; b = data & 0x10;
    h = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    return data;
  }

  inline auto operator|=(uint8 data) -> uint { return operator=(operator uint() | data); }
  inline auto operator^=(uint8 data) -> uint { return operator=(operator uint() ^ data); }
  inline auto operator&=(uint8 data) -> uint { return operator=(operator uint() & data); }

  bool n, v, p, b, h, i, z, c;
};

struct Word {
  inline operator uint() const { return w; }
  inline auto operator=(uint data) -> uint { return w = data; }

  inline auto operator++() -> uint { return ++w; }
  inline auto operator--() -> uint { return --w; }

  inline auto operator++(int) -> uint { unsigned data = w++; return data; }
  inline auto operator--(int) -> uint { unsigned data = w--; return data; }

  inline auto operator+=(uint data) -> uint { return w += data;; }
  inline auto operator-=(uint data) -> uint { return w -= data;; }

  inline auto operator|=(uint data) -> uint { return w |= data; }
  inline auto operator^=(uint data) -> uint { return w ^= data; }
  inline auto operator&=(uint data) -> uint { return w &= data; }

  union {
    uint16_t w;
    struct { uint8_t order_lsb2(l, h); };
  };
};

struct Regs {
  Word pc;
  union {
    uint16_t ya;
    struct { uint8_t order_lsb2(a, y); };
  };
  uint8_t x, s;
  Flag p;
};
