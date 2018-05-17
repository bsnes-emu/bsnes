//MCC - Memory Controller Chip
//Custom logic chip inside the BS-X Satellaview base cartridge

struct MCC {
  MappedRAM rom;
  MappedRAM psram;

  //mcc.cpp
  auto unload() -> void;
  auto power() -> void;

  auto read(uint24 address, uint8 data) -> uint8;
  auto write(uint24 address, uint8 data) -> void;

  auto mcuRead(uint24 address, uint8 data) -> uint8;
  auto mcuWrite(uint24 address, uint8 data) -> void;

  auto mcuAccess(bool mode, uint24 address, uint8 data) -> uint8;
  auto romAccess(bool mode, uint24 address, uint8 data) -> uint8;
  auto psramAccess(bool mode, uint24 address, uint8 data) -> uint8;
  auto exAccess(bool mode, uint24 address, uint8 data) -> uint8;
  auto bsAccess(bool mode, uint24 address, uint8 data) -> uint8;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct IRQ {
    uint1 flag;    //bit 0
    uint1 enable;  //bit 1
  } irq;

  struct Registers {
    uint1 mapping;        //bit  2 (0 = ignore A15; 1 = use A15)
    uint1 psramEnableLo;  //bit  3
    uint1 psramEnableHi;  //bit  4
    uint2 psramMapping;   //bits 5-6
    uint1 romEnableLo;    //bit  7
    uint1 romEnableHi;    //bit  8
    uint1 exEnableLo;     //bit  9
    uint1 exEnableHi;     //bit 10
    uint1 exMapping;      //bit 11
    uint1 bsWritable;     //bit 12
    uint1 unknown;        //bit 13
  } r, w;

  //bit 14 (commit)

  struct ExtendedRegisters {
    uint1 enable;  //bit  15
    uint8 value;   //bits 24-31
  } x;
};

extern MCC mcc;
