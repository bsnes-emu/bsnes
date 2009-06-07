struct flag_t {
  bool n, v, p, b, h, i, z, c;

  inline operator unsigned() const {
    return (n << 7) + (v << 6) + (p << 5) + (b << 4)
         + (h << 3) + (i << 2) + (z << 1) + (c << 0);
  }

  inline unsigned operator=(uint8_t data) {
    n = data & 0x80; v = data & 0x40; p = data & 0x20; b = data & 0x10;
    h = data & 0x08; i = data & 0x04; z = data & 0x02; c = data & 0x01;
    return data;
  }

  inline unsigned operator|=(unsigned data) { return operator=(operator unsigned() | data); }
  inline unsigned operator^=(unsigned data) { return operator=(operator unsigned() ^ data); }
  inline unsigned operator&=(unsigned data) { return operator=(operator unsigned() & data); }

  flag_t() : n(0), v(0), p(0), b(0), h(0), i(0), z(0), c(0) {}
};

struct regs_t {
  uint16_t pc;
  union {
    uint16 ya;
    struct { uint8 order_lsb2(a, y); };
  };
  uint8_t x, sp;
  flag_t p;
  regs_t() : pc(0), ya(0), x(0), sp(0) {}
};
