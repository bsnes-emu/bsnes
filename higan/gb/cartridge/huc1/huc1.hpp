struct HuC1 : Mapper {
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;

  struct IO {
    uint1 model;
    struct ROM {
      uint8 bank = 0x01;
    } rom;
    struct RAM {
      uint1 writable;
      uint8 bank;
    } ram;
  } io;
} huc1;
