struct Flags {
  union {
    uint8_t b = 0;
    BooleanBitField<uint8_t, 7> n;
    BooleanBitField<uint8_t, 6> v;
    BooleanBitField<uint8_t, 5> m;
    BooleanBitField<uint8_t, 4> x;
    BooleanBitField<uint8_t, 3> d;
    BooleanBitField<uint8_t, 2> i;
    BooleanBitField<uint8_t, 1> z;
    BooleanBitField<uint8_t, 0> c;
  };

  inline operator uint() const { return b; }
  inline auto operator =(uint value) -> uint { return b  = value; }
  inline auto operator&=(uint value) -> uint { return b &= value; }
  inline auto operator|=(uint value) -> uint { return b |= value; }
};

struct Reg16 {
  union {
    uint16_t w = 0;
    NaturalBitField<uint16_t, 0,  7> l;
    NaturalBitField<uint16_t, 8, 15> h;
  };

  inline operator uint() const { return w; }
  inline auto operator  =(uint value) -> uint { return w   = value; }
  inline auto operator |=(uint value) -> uint { return w  |= value; }
  inline auto operator ^=(uint value) -> uint { return w  ^= value; }
  inline auto operator &=(uint value) -> uint { return w  &= value; }
  inline auto operator<<=(uint value) -> uint { return w <<= value; }
  inline auto operator>>=(uint value) -> uint { return w >>= value; }
  inline auto operator +=(uint value) -> uint { return w  += value; }
  inline auto operator -=(uint value) -> uint { return w  -= value; }
  inline auto operator *=(uint value) -> uint { return w  *= value; }
  inline auto operator /=(uint value) -> uint { return w  /= value; }
  inline auto operator %=(uint value) -> uint { return w  %= value; }
};

struct Reg24 {
  union {
    uint32_t d = 0;
    NaturalBitField<uint32_t,  0,  7> l;
    NaturalBitField<uint32_t,  8, 15> h;
    NaturalBitField<uint32_t, 16, 23> b;
    NaturalBitField<uint32_t,  0, 15> w;
  };

  inline operator uint() const { return d; }
  inline auto operator  =(uint value) -> uint { return d = uint24(     value); }
  inline auto operator |=(uint value) -> uint { return d = uint24(d  | value); }
  inline auto operator ^=(uint value) -> uint { return d = uint24(d  ^ value); }
  inline auto operator &=(uint value) -> uint { return d = uint24(d  & value); }
  inline auto operator<<=(uint value) -> uint { return d = uint24(d << value); }
  inline auto operator>>=(uint value) -> uint { return d = uint24(d >> value); }
  inline auto operator +=(uint value) -> uint { return d = uint24(d  + value); }
  inline auto operator -=(uint value) -> uint { return d = uint24(d  - value); }
  inline auto operator *=(uint value) -> uint { return d = uint24(d  * value); }
  inline auto operator /=(uint value) -> uint { return d = uint24(d  / value); }
  inline auto operator %=(uint value) -> uint { return d = uint24(d  % value); }
};

struct Registers {
  Reg24 pc;
  Reg16 a;
  Reg16 x;
  Reg16 y;
  Reg16 z;  //pseudo-register (zero register)
  Reg16 s;
  Reg16 d;
  Flags p;
  uint8 db = 0;
  bool e = false;

  bool irq = false;   //IRQ pin (0 = low, 1 = trigger)
  bool wai = false;   //raised during wai, cleared after interrupt triggered
  bool stp = false;   //raised during stp, never cleared
  uint8 mdr = 0;      //memory data register
  uint16 vector = 0;  //interrupt vector address
};
