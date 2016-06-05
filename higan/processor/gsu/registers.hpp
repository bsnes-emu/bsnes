struct Register {
  uint16 data = 0;
  bool modified = false;

  inline operator uint() const {
    return data;
  }

  inline auto assign(uint value) -> uint16 {
    modified = true;
    return data = value;
  }

  inline auto operator++() { return assign(data + 1); }
  inline auto operator--() { return assign(data - 1); }
  inline auto operator++(int) { uint r = data; assign(data + 1); return r; }
  inline auto operator--(int) { uint r = data; assign(data - 1); return r; }
  inline auto operator   = (uint i) { return assign(i); }
  inline auto operator  |= (uint i) { return assign(data | i); }
  inline auto operator  ^= (uint i) { return assign(data ^ i); }
  inline auto operator  &= (uint i) { return assign(data & i); }
  inline auto operator <<= (uint i) { return assign(data << i); }
  inline auto operator >>= (uint i) { return assign(data >> i); }
  inline auto operator  += (uint i) { return assign(data + i); }
  inline auto operator  -= (uint i) { return assign(data - i); }
  inline auto operator  *= (uint i) { return assign(data * i); }
  inline auto operator  /= (uint i) { return assign(data / i); }
  inline auto operator  %= (uint i) { return assign(data % i); }

  inline auto operator   = (const Register& value) { return assign(value); }

  Register() = default;
  Register(const Register&) = delete;
};

struct SFR {
  bool irq;   //interrupt flag
  bool b;     //WITH flag
  bool ih;    //immediate higher 8-bit flag
  bool il;    //immediate lower 8-bit flag
  bool alt2;  //ALT2 mode
  bool alt1;  //ALT2 instruction mode
  bool r;     //ROM r14 read flag
  bool g;     //GO flag
  bool ov;    //overflow flag
  bool s;     //sign flag
  bool cy;    //carry flag
  bool z;     //zero flag

  operator uint() const {
    return (irq << 15) | (b << 12) | (ih << 11) | (il << 10) | (alt2 << 9) | (alt1 << 8)
         | (r << 6) | (g << 5) | (ov << 4) | (s << 3) | (cy << 2) | (z << 1);
  }

  auto& operator=(uint data) {
    irq  = data & 0x8000;
    b    = data & 0x1000;
    ih   = data & 0x0800;
    il   = data & 0x0400;
    alt2 = data & 0x0200;
    alt1 = data & 0x0100;
    r    = data & 0x0040;
    g    = data & 0x0020;
    ov   = data & 0x0010;
    s    = data & 0x0008;
    cy   = data & 0x0004;
    z    = data & 0x0002;
    return *this;
  }
};

struct SCMR {
  uint ht;
  bool ron;
  bool ran;
  uint md;

  operator uint() const {
    return ((ht >> 1) << 5) | (ron << 4) | (ran << 3) | ((ht & 1) << 2) | (md);
  }

  auto& operator=(uint data) {
    ht  = (bool)(data & 0x20) << 1;
    ht |= (bool)(data & 0x04) << 0;
    ron = data & 0x10;
    ran = data & 0x08;
    md  = data & 0x03;
    return *this;
  }
};

struct POR {
  bool obj;
  bool freezehigh;
  bool highnibble;
  bool dither;
  bool transparent;

  operator uint() const {
    return (obj << 4) | (freezehigh << 3) | (highnibble << 2) | (dither << 1) | (transparent);
  }

  auto& operator=(uint data) {
    obj         = data & 0x10;
    freezehigh  = data & 0x08;
    highnibble  = data & 0x04;
    dither      = data & 0x02;
    transparent = data & 0x01;
    return *this;
  }
};

struct CFGR {
  bool irq;
  bool ms0;

  operator uint() const {
    return (irq << 7) | (ms0 << 5);
  }

  auto& operator=(uint data) {
    irq = data & 0x80;
    ms0 = data & 0x20;
    return *this;
  }
};

struct Registers {
  uint8 pipeline;
  uint16 ramaddr;

  Register r[16];   //general purpose registers
  SFR sfr;          //status flag register
  uint8 pbr;        //program bank register
  uint8 rombr;      //game pack ROM bank register
  bool rambr;       //game pack RAM bank register
  uint16 cbr;       //cache base register
  uint8 scbr;       //screen base register
  SCMR scmr;        //screen mode register
  uint8 colr;       //color register
  POR por;          //plot option register
  bool bramr;       //back-up RAM register
  uint8 vcr;        //version code register
  CFGR cfgr;        //config register
  bool clsr;        //clock select register

  uint romcl;       //clock ticks until romdr is valid
  uint8 romdr;      //ROM buffer data register

  uint ramcl;       //clock ticks until ramdr is valid
  uint16 ramar;     //RAM buffer address register
  uint8 ramdr;      //RAM buffer data register

  uint sreg;
  uint dreg;
  auto& sr() { return r[sreg]; }  //source register (from)
  auto& dr() { return r[dreg]; }  //destination register (to)

  auto reset() -> void {
    sfr.b    = 0;
    sfr.alt1 = 0;
    sfr.alt2 = 0;

    sreg = 0;
    dreg = 0;
  }
} regs;

struct Cache {
  uint8 buffer[512];
  bool valid[32];
} cache;

struct PixelCache {
  uint16 offset;
  uint8 bitpend;
  uint8 data[8];
} pixelcache[2];
