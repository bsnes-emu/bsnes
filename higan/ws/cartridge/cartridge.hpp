struct Cartridge {
  auto loaded() const -> bool;

  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;

  auto romRead(uint addr) -> uint8;
  auto romWrite(uint addr, uint8 data) -> void;

  auto ramRead(uint addr) -> uint8;
  auto ramWrite(uint addr, uint8 data) -> void;

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

  struct Information {
    string manifest;
    string title;
    string sha256;
  } information;

privileged:
  bool _loaded = false;
};

extern Cartridge cartridge;
