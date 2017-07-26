struct MBC1M : Mapper {
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;

  struct IO {
    uint1 mode;
    struct ROM {
      uint6 bank = 0x01;
    } rom;
  } io;
} mbc1m;
