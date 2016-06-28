struct MBC3 : MMIO {
  auto second() -> void;
  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;
  auto power() -> void;

  struct ROM {
    uint8 select;
  } rom;
  struct RAM {
    bool enable;
    uint8 select;
  } ram;
  struct RTC {
    bool latch;

    bool halt;
    uint second;
    uint minute;
    uint hour;
    uint day;
    bool dayCarry;

    uint latchSecond;
    uint latchMinute;
    uint latchHour;
    uint latchDay;
    uint latchDayCarry;
  } rtc;
} mbc3;
