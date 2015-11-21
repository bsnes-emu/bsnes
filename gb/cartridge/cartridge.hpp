struct Cartridge : MMIO, property<Cartridge> {
  Cartridge();
  ~Cartridge();

  auto load_empty(System::Revision revision) -> void;
  auto load(System::Revision revision) -> void;
  auto unload() -> void;

  auto rom_read(uint addr) -> uint8;
  auto rom_write(uint addr, uint8 data) -> void;
  auto ram_read(uint addr) -> uint8;
  auto ram_write(uint addr, uint8 data) -> void;

  auto mmio_read(uint16 addr) -> uint8;
  auto mmio_write(uint16 addr, uint8 data) -> void;

  auto power() -> void;

  auto serialize(serializer&) -> void;

  #include "mbc0/mbc0.hpp"
  #include "mbc1/mbc1.hpp"
  #include "mbc2/mbc2.hpp"
  #include "mbc3/mbc3.hpp"
  #include "mbc5/mbc5.hpp"
  #include "mmm01/mmm01.hpp"
  #include "huc1/huc1.hpp"
  #include "huc3/huc3.hpp"

  enum Mapper : uint {
    MBC0,
    MBC1,
    MBC2,
    MBC3,
    MBC5,
    MMM01,
    HuC1,
    HuC3,
    Unknown,
  };

  struct Information {
    string markup;
    string title;

    Mapper mapper;
    bool ram;
    bool battery;
    bool rtc;
    bool rumble;

    uint romsize;
    uint ramsize;
  } information;

  string title();

  struct Memory {
    uint id;
    string name;
  };
  vector<Memory> memory;

  readonly<bool> loaded;
  readonly<string> sha256;

  uint8* romdata = nullptr;
  uint romsize = 0;

  uint8* ramdata = nullptr;
  uint ramsize = 0;

  MMIO* mapper = nullptr;
  bool bootrom_enable = true;
};

extern Cartridge cartridge;
