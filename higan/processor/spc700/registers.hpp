struct Flags {
  union {
    uint8_t data = 0;
    BooleanBitField<uint8_t, 7> n;
    BooleanBitField<uint8_t, 6> v;
    BooleanBitField<uint8_t, 5> p;
    BooleanBitField<uint8_t, 4> b;
    BooleanBitField<uint8_t, 3> h;
    BooleanBitField<uint8_t, 2> i;
    BooleanBitField<uint8_t, 1> z;
    BooleanBitField<uint8_t, 0> c;
  };

  inline operator uint() const { return data; }
  inline auto& operator =(uint value) { return data  = value, *this; }
  inline auto& operator&=(uint value) { return data &= value, *this; }
  inline auto& operator|=(uint value) { return data |= value, *this; }
  inline auto& operator^=(uint value) { return data ^= value, *this; }
};

struct Register {
  union {
    uint16_t w = 0;
    NaturalBitField<uint16_t, 0,  7> l;
    NaturalBitField<uint16_t, 8, 15> h;
  };

  inline operator uint() const { return w; }
  inline auto operator=(const Register& value) { w = value.w; }

  inline auto operator++(int) { uint value = w++; return value; }
  inline auto operator--(int) { uint value = w--; return value; }

  inline auto& operator++() { return ++w, *this; }
  inline auto& operator--() { return --w, *this; }

  inline auto& operator =(uint value) { return w  = value, *this; }
  inline auto& operator&=(uint value) { return w &= value, *this; }
  inline auto& operator|=(uint value) { return w |= value, *this; }
  inline auto& operator^=(uint value) { return w ^= value, *this; }
  inline auto& operator+=(uint value) { return w += value, *this; }
  inline auto& operator-=(uint value) { return w -= value, *this; }
};

struct Registers {
  Register pc;
  union {
    uint16_t ya;
    struct { uint8_t order_lsb2(a, y); };
  };
  uint8_t x, s;
  Flags p;
};
