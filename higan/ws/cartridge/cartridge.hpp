struct Cartridge : IO {
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto romRead(uint20 addr) -> uint8;
  auto romWrite(uint20 addr, uint8 data) -> void;

  auto ramRead(uint20 addr) -> uint8;
  auto ramWrite(uint20 addr, uint8 data) -> void;

  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  struct Registers {
    uint8 bank_rom0;
    uint8 bank_rom1;
    uint8 bank_rom2;
    uint8 bank_sram;
  } r;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
    uint mask = 0;
    string name;
  } rom, ram;

  EEPROM eeprom;

  struct Information {
    string manifest;
    string title;
    string sha256;
  } information;
};

extern Cartridge cartridge;
