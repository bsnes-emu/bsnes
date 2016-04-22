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
  auto read(uint24 addr) -> uint8 override;
  auto write(uint24 addr, uint8 data) -> void override;

  //CPU ROM read/write
  auto romRead(uint24 addr, uint8 data) -> uint8;
  auto romWrite(uint24 addr, uint8 data) -> void;

  //CPU RAM read/write
  auto ramRead(uint24 addr, uint8 data) -> uint8;
  auto ramWrite(uint24 addr, uint8 data) -> void;

  //HG51B data RAM read/write
  auto dramRead(uint24 addr, uint8 data) -> uint8;
  auto dramWrite(uint24 addr, uint8 data) -> void;

  //CPU MMIO read/write
  auto dspRead(uint24 addr, uint8 data) -> uint8;
  auto dspWrite(uint24 addr, uint8 data) -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;

  uint Frequency;
  uint Roms;
};

extern HitachiDSP hitachidsp;
