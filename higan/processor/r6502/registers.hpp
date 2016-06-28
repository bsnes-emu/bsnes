struct Flags {
  union {
    uint8_t data = 0;
    BooleanBitField<uint8_t, 7> n;
    BooleanBitField<uint8_t, 6> v;
    BooleanBitField<uint8_t, 3> d;
    BooleanBitField<uint8_t, 2> i;
    BooleanBitField<uint8_t, 1> z;
    BooleanBitField<uint8_t, 0> c;
  };

  inline operator uint() { return data; }
  inline auto& operator =(uint value) { return data  = value, *this; }
  inline auto& operator&=(uint value) { return data &= value, *this; }
  inline auto& operator|=(uint value) { return data |= value, *this; }
  inline auto& operator^=(uint value) { return data ^= value, *this; }
};

struct Registers {
  uint8  mdr;
  uint16 pc;
  uint8  a, x, y, s;
  Flags  p;
} regs;

struct Register16 {
  union {
    uint16_t w;
    NaturalBitField<uint16_t, 0,  7> l;
    NaturalBitField<uint16_t, 8, 15> h;
  };
} abs, iabs;

uint8 rd;
uint8 zp;
uint16 aa;
