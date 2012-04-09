struct EEPROM {
  bool data[64 * 1024];
  unsigned size;

  enum class Mode : unsigned { Wait, Command, ReadAddress, ReadValidate, ReadData, WriteAddress, WriteData, WriteValidate } mode;
  unsigned offset;
  unsigned address;

  bool read();
  void write(bool bit);
  void power();
} eeprom;
