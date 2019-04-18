struct MBC3 : Mapper {
  auto second() -> void;
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
  auto power() -> void;
  auto serialize(serializer& s) -> void;

  struct IO {
    struct ROM {
      uint8 bank = 0x01;
    } rom;
    struct RAM {
      uint1 enable;
      uint8 bank;
    } ram;
    struct RTC {
      uint1 halt = true;
      uint1 latch;

      uint8 second;
      uint8 minute;
      uint8 hour;
      uint9 day;
      uint1 dayCarry;

      uint8 latchSecond;
      uint8 latchMinute;
      uint8 latchHour;
      uint9 latchDay;
      uint1 latchDayCarry;
    } rtc;
  } io;
} mbc3;
