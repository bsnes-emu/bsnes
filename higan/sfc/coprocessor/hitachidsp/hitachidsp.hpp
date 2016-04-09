struct HitachiDSP : Processor::HG51B, Cothread {
  MappedRAM rom;
  MappedRAM ram;

  #include "mmio.hpp"

  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;

  //HG51B read/write
  auto bus_read(uint24 addr) -> uint8;
  auto bus_write(uint24 addr, uint8 data) -> void;

  //CPU ROM read/write
  auto rom_read(uint24 addr, uint8 data) -> uint8;
  auto rom_write(uint24 addr, uint8 data) -> void;

  //CPU RAM read/write
  auto ram_read(uint24 addr, uint8 data) -> uint8;
  auto ram_write(uint24 addr, uint8 data) -> void;

  //HG51B data RAM read/write
  auto dram_read(uint24 addr, uint8 data) -> uint8;
  auto dram_write(uint24 addr, uint8 data) -> void;

  //CPU MMIO read/write
  auto dsp_read(uint24 addr, uint8 data) -> uint8;
  auto dsp_write(uint24 addr, uint8 data) -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;

  uint Frequency;
  uint Roms;
};

extern HitachiDSP hitachidsp;
