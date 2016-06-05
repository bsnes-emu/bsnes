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
  auto stop() -> void;
  auto color(uint8 source) -> uint8;
  auto plot(uint8 x, uint8 y) -> void;
  auto rpix(uint8 x, uint8 y) -> uint8;
  auto pixelcache_flush(pixelcache_t& cache) -> void;

  //memory.cpp
  auto bus_read(uint24 addr, uint8 data = 0x00) -> uint8 override;
  auto bus_write(uint24 addr, uint8 data) -> void override;

  auto op_read(uint16 addr) -> uint8;
  alwaysinline auto peekpipe() -> uint8;
  alwaysinline auto pipe() -> uint8;

  auto cache_flush() -> void;
  auto cache_mmio_read(uint16 addr) -> uint8;
  auto cache_mmio_write(uint16 addr, uint8 data) -> void;

  auto memory_reset() -> void;

  //mmio.cpp
  auto readIO(uint24 addr, uint8 data) -> uint8;
  auto writeIO(uint24 addr, uint8 data) -> void;

  //timing.cpp
  auto step(uint clocks) -> void;

  auto rombuffer_sync() -> void;
  auto rombuffer_update() -> void;
  auto rombuffer_read() -> uint8;

  auto rambuffer_sync() -> void;
  auto rambuffer_read(uint16 addr) -> uint8;
  auto rambuffer_write(uint16 addr, uint8 data) -> void;

  auto r14_modify(uint16) -> void;
  auto r15_modify(uint16) -> void;

  auto timing_reset() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  CPUROM cpurom;
  CPURAM cpuram;

private:
  uint rom_mask;  //rom_size - 1
  uint ram_mask;  //ram_size - 1

  bool r15_modified = false;
};

extern SuperFX superfx;
