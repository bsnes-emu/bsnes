struct Memory {
  auto ror(uint32 word, unsigned shift) -> uint32 {
    return word << 32 - shift | word >> shift;
  }
};

struct MROM : Memory {
  uint8* data;
  unsigned size;
  unsigned mask;

  auto read(unsigned mode, uint32 addr) -> uint32;
  auto write(unsigned mode, uint32 addr, uint32 word) -> void;
} mrom;

struct SRAM : Memory {
  uint8* data;
  unsigned size;
  unsigned mask;

  auto read(unsigned mode, uint32 addr) -> uint32;
  auto write(unsigned mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;
} sram;

struct EEPROM : Memory {
  uint8* data;
  unsigned size;
  unsigned mask;
  unsigned test;
  unsigned bits;

  enum class Mode : unsigned {
    Wait, Command, ReadAddress, ReadValidate, ReadData, WriteAddress, WriteData, WriteValidate
  } mode;
  unsigned offset;
  unsigned address;
  unsigned addressbits;

  auto read(unsigned addr) -> bool;
  auto write(unsigned addr, bool bit) -> void;

  auto read() -> bool;
  auto write(bool bit) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;
} eeprom;

struct FLASH : Memory {
  uint8* data;
  unsigned size;
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
