struct IO {
  virtual auto portRead(uint16 addr) -> uint8 = 0;
  virtual auto portWrite(uint16 addr, uint8 data) -> void = 0;
};

struct InternalRAM {
  auto power() -> void;
  auto serialize(serializer&) -> void;

  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;

  //PPU byte reads only:
  //WS: address is always < 0x4000
  alwaysinline auto read8(uint16 address) const -> uint16 {
    return memory[address];
  }

  //PPU word reads only:
  //address & 1 is always 0
  //WS: address is always < 0x4000
  alwaysinline auto read16(uint16 address) const -> uint16 {
    return memory[address + 0] << 0 | memory[address + 1] << 8;
  }

  //PPU long reads only:
  //address & 3 is always 0
  //WS: address is always < 0x4000
  alwaysinline auto read32(uint16 address) const -> uint32 {
    return memory[address + 0] <<  0 | memory[address + 1] <<  8
         | memory[address + 2] << 16 | memory[address + 3] << 24;
  }

private:
  uint8 memory[65536];
};

struct Bus {
  auto power() -> void;

  auto read(uint20 addr) -> uint8;
  auto write(uint20 addr, uint8 data) -> void;

  auto map(IO* io, uint16_t lo, maybe<uint16_t> hi = nothing) -> void;
  auto portRead(uint16 addr) -> uint8;
  auto portWrite(uint16 addr, uint8 data) -> void;

private:
  IO* port[64 * 1024] = {nullptr};
};

extern InternalRAM iram;
extern Bus bus;
