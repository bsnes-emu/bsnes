struct SuperFX : Processor::GSU, Cothread {
  MappedRAM rom;
  MappedRAM ram;

  //superfx.cpp
  static auto Enter() -> void;
  auto main() -> void;
  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //bus.cpp
  struct CPUROM : Memory {
    auto size() const -> uint;
    auto read(uint24, uint8) -> uint8;
    auto write(uint24, uint8) -> void;
  };

  struct CPURAM : Memory {
    auto size() const -> uint;
    auto read(uint24, uint8) -> uint8;
    auto write(uint24, uint8) -> void;
  };

  //core.cpp
  auto stop() -> void override;
  auto color(uint8 source) -> uint8 override;
  auto plot(uint8 x, uint8 y) -> void override;
  auto rpix(uint8 x, uint8 y) -> uint8 override;

  auto flushPixelCache(PixelCache& cache) -> void;

  //memory.cpp
  auto read(uint24 addr, uint8 data = 0x00) -> uint8 override;
  auto write(uint24 addr, uint8 data) -> void override;

  auto readOpcode(uint16 addr) -> uint8;
  alwaysinline auto peekpipe() -> uint8;
  alwaysinline auto pipe() -> uint8 override;

  auto flushCache() -> void override;
  auto readCache(uint16 addr) -> uint8;
  auto writeCache(uint16 addr, uint8 data) -> void;

  //mmio.cpp
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;

  //timing.cpp
  auto step(uint clocks) -> void override;

  auto syncROMBuffer() -> void override;
  auto readROMBuffer() -> uint8 override;
  auto updateROMBuffer() -> void;

  auto syncRAMBuffer() -> void override;
  auto readRAMBuffer(uint16 addr) -> uint8 override;
  auto writeRAMBuffer(uint16 addr, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  CPUROM cpurom;
  CPURAM cpuram;

private:
  uint romMask;
  uint ramMask;
};

extern SuperFX superfx;
