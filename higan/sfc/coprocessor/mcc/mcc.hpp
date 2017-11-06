//the MCC is the custom logic chip inside the BS-X Satellaview cartridge

struct MCC {
  MappedRAM rom;
  MappedRAM ram;

  auto unload() -> void;
  auto power() -> void;

  auto memoryAccess(bool write, Memory& memory, uint24 addr, uint8 data) -> uint8;
  auto mcuAccess(bool write, uint24 addr, uint8 data) -> uint8;

  auto mcuRead(uint24 addr, uint8 data) -> uint8;
  auto mcuWrite(uint24 addr, uint8 data) -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto commit() -> void;

  auto serialize(serializer&) -> void;

private:
  uint8 r[16];
  bool r00, r01, r02, r03;
  bool r04, r05, r06, r07;
  bool r08, r09, r0a, r0b;
  bool r0c, r0d, r0e, r0f;
};

extern MCC mcc;
