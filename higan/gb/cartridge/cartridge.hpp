struct Cartridge : MMIO {
  auto pathID() const -> uint { return information.pathID; }
  auto sha256() const -> string { return information.sha256; }
  auto manifest() const -> string { return information.manifest; }
  auto title() const -> string { return information.title; }

  auto load(System::Revision revision) -> bool;
  auto save() -> void;
  auto unload() -> void;

  auto readROM(uint addr) -> uint8;
  auto writeROM(uint addr, uint8 data) -> void;

  auto readRAM(uint addr) -> uint8;
  auto writeRAM(uint addr, uint8 data) -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

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
    string sha256;
    string manifest;
    string title;

    Mapper mapper = Mapper::Unknown;
    boolean ram;
    boolean battery;
    boolean rtc;
    boolean rumble;
  } information;

  struct Memory {
    uint8* data = nullptr;
    uint size = 0;
  } rom, ram;

  MMIO* mapper = nullptr;
  bool bootromEnable = true;
};

extern Cartridge cartridge;
