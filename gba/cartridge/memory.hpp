struct MROM {
  uint8* data;
  uint size;
  uint mask;

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;
} mrom;

struct SRAM {
  uint8* data;
  uint size;
  uint mask;

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;
} sram;

struct EEPROM {
  uint8* data;
  uint size;
  uint mask;
  uint test;
  uint bits;

  enum class Mode : uint {
    Wait, Command, ReadAddress, ReadValidate, ReadData, WriteAddress, WriteData, WriteValidate
  } mode;
  uint offset;
  uint address;
  uint addressbits;

  auto read(uint addr) -> bool;
  auto write(uint addr, bool bit) -> void;

  auto read() -> bool;
  auto write(bool bit) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;
} eeprom;

struct FLASH {
  uint8* data;
  uint size;
  uint16 id;

  bool unlockhi;
  bool unlocklo;
  bool idmode;
  bool erasemode;
  bool bankselect;
  bool writeselect;
  bool bank;

  auto read(uint16 addr) -> uint8;
  auto write(uint16 addr, uint8 byte) -> void;

  auto power() -> void;
  auto serialize(serializer&) -> void;
} flash;
