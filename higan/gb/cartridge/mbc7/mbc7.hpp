struct MBC7 : Mapper {
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto power() -> void;
  auto serialize(serializer&) -> void;

  struct IO {
    struct ROM {
      uint8 bank = 0x01;
    } rom;
    struct RAM {
      uint1 enable[2];
    } ram;
    struct Accelerometer {
      uint16 x = 0x8000;
      uint16 y = 0x8000;
      uint16 z = 0xff00;  //unused
    } accelerometer;
  } io;
} mbc7;
