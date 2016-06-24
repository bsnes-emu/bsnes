struct Cartridge : MMIO, property<Cartridge> {
  auto pathID() const -> uint { return information.pathID; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load(System::Revision revision) -> bool;
  auto save() -> void;
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
  #include "mbc1m/mbc1m.hpp"
  #include "mbc2/mbc2.hpp"
  #include "mbc3/mbc3.hpp"
  #include "mbc5/mbc5.hpp"
  #include "mmm01/mmm01.hpp"
  #include "huc1/huc1.hpp"
  #include "huc3/huc3.hpp"

  enum Mapper : uint {
    MBC0,
    MBC1,
    MBC1M,
    MBC2,
    MBC3,
    MBC5,
    MMM01,
    HuC1,
    HuC3,
    Unknown,
  };

  struct Information {
    uint pathID = 0;
    string manifest;
    string title;

    Mapper mapper = Mapper::Unknown;
    boolean ram;
    boolean battery;
    boolean rtc;
    boolean rumble;

    uint romsize = 0;
    uint ramsize = 0;
  } information;

  uint _pathID = 0;
  readonly<string> sha256;

  uint8* romdata = nullptr;
  uint romsize = 0;

  uint8* ramdata = nullptr;
  uint ramsize = 0;

  MMIO* mapper = nullptr;
  bool bootrom_enable = true;
};

extern Cartridge cartridge;
