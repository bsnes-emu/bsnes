struct MBC1 : Mapper {
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto power() -> void;
  auto serialize(serializer& s) -> void;

  struct IO {
    uint1 mode;
    struct ROM {
      uint8 bank = 0x01;
    } rom;
    struct RAM {
      uint1 enable;
      uint8 bank;
    } ram;
  } io;
} mbc1;
