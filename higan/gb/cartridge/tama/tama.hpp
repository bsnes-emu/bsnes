struct TAMA : Mapper {
  auto second() -> void;
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;
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

  struct RTC {
    uint8 year;      //0 - 99
    uint8 month;     //1 - 12
    uint8 day;       //1 - 31
    uint8 hour;      //0 - 23
    uint8 minute;    //0 - 59
    uint8 second;    //0 - 59
    uint1 meridian;  //0 = AM; 1 = PM
    uint2 leapYear;  //0 = leap year; 1-3 = non-leap year
    uint1 hourMode;  //0 = 12-hour; 1 = 24-hour
    uint4 test;
    uint8 index;
  } rtc;
} tama;
