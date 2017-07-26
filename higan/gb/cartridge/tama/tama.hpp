struct TAMA : Mapper {
  auto second() -> void;
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto readRTC(uint1 page, uint4 address) -> uint4;
  auto writeRTC(uint1 page, uint4 address, uint4 data) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;

  struct IO {
    uint1 ready;
    uint4 select;
    uint3 mode;
    uint5 index;
    uint8 input;
    uint8 output;
    struct ROM {
      uint5 bank;
    } rom;
  } io;
} tama;
