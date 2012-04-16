struct Memory {
  uint8 *data;
  unsigned size;
} rom, ram;

struct EEPROM {
  bitarray data;
  unsigned size;
  unsigned mask;
  unsigned test;
  unsigned bits;

  enum class Mode : unsigned { Wait, Command, ReadAddress, ReadValidate, ReadData, WriteAddress, WriteData, WriteValidate } mode;
  unsigned offset;
  unsigned address;

  bool read();
  void write(bool bit);
  void power();
} eeprom;

struct FlashROM {
  uint8 *data;
  unsigned size;
  uint16 id;

  bool unlockhi;
  bool unlocklo;
  bool idmode;
  bool erasemode;
  bool bankselect;
  bool writeselect;
  bool bank;

  uint8 read(uint16 addr);
  void write(uint16 addr, uint8 byte);
  void power();
} flashrom;
