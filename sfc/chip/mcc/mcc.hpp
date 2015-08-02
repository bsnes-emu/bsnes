//the MCC is the custom logic chip inside the BS-X Satellaview cartridge

struct MCC {
  MappedRAM rom;
  MappedRAM ram;
  MappedRAM psram;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  auto memory_access(bool write, Memory& memory, unsigned addr, uint8 data) -> uint8;
  auto memory_read(Memory& memory, unsigned addr) -> uint8;
  auto memory_write(Memory& memory, unsigned addr, uint8 data) -> void;

  auto mcu_access(bool write, unsigned addr, uint8 data = 0x00) -> uint8;
  auto mcu_read(unsigned addr) -> uint8;
  auto mcu_write(unsigned addr, uint8 data) -> void;

  auto mmio_read(unsigned addr) -> uint8;
  auto mmio_write(unsigned addr, uint8 data) -> void;
  auto mmio_commit() -> void;

  auto serialize(serializer&) -> void;

private:
  uint8 r[16];
  bool r00, r01, r02, r03;
  bool r04, r05, r06, r07;
  bool r08, r09, r0a, r0b;
  bool r0c, r0d, r0e, r0f;
};

extern MCC mcc;
