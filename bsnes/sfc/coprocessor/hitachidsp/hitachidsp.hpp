struct HitachiDSP : Processor::HG51B, Thread {
  ReadableMemory rom;
  WritableMemory ram;

  //hitachidsp.cpp
  static auto Enter() -> void;
  auto step(uint clocks) -> void override;
  auto halt() -> void override;

  auto unload() -> void;
  auto power() -> void;

  auto isROM(uint24 address) -> bool override;
  auto isRAM(uint24 address) -> bool override;

  //HG51B read/write
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //CPU ROM read/write
  auto addressROM(uint24 address) const -> maybe<uint24>;
  auto readROM(uint24 address, uint8 data = 0) -> uint8;
  auto writeROM(uint24 address, uint8 data) -> void;

  //CPU RAM read/write
  auto addressRAM(uint24 address) const -> maybe<uint24>;
  auto readRAM(uint24 address, uint8 data = 0) -> uint8;
  auto writeRAM(uint24 address, uint8 data) -> void;

  //HG51B data RAM read/write
  auto addressDRAM(uint24 address) const -> maybe<uint24>;
  auto readDRAM(uint24 address, uint8 data = 0) -> uint8;
  auto writeDRAM(uint24 address, uint8 data) -> void;

  //CPU IO read/write
  auto addressIO(uint24 address) const -> maybe<uint24>;
  auto readIO(uint24 address, uint8 data = 0) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  auto firmware() const -> vector<uint8>;
  auto serialize(serializer&) -> void;

  uint Frequency;
  uint Roms;
  bool Mapping;
};

extern HitachiDSP hitachidsp;
