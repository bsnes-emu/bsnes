//Epson RTC-4513 Real-Time Clock

struct EpsonRTC : Cothread {
  static auto Enter() -> void;
  auto main() -> void;

  auto init() -> void;
  auto load() -> void;
  auto unload() -> void;
  auto power() -> void;
  auto reset() -> void;
  auto sync() -> void;

  auto read(uint24 addr, uint8 data) -> uint8;
  auto write(uint24 addr, uint8 data) -> void;

  auto serialize(serializer&) -> void;

  uint21 clocks;
  uint seconds;

  uint2 chipselect;
  enum class State : uint { Mode, Seek, Read, Write } state;
  uint4 mdr;
  uint4 offset;
  uint wait;
  uint1 ready;
  uint1 holdtick;

  uint4 secondlo;
  uint3 secondhi;
  uint1 batteryfailure;

  uint4 minutelo;
  uint3 minutehi;
  uint1 resync;

  uint4 hourlo;
  uint2 hourhi;
  uint1 meridian;

  uint4 daylo;
  uint2 dayhi;
  uint1 dayram;

  uint4 monthlo;
  uint1 monthhi;
  uint2 monthram;

  uint4 yearlo;
  uint4 yearhi;

  uint3 weekday;

  uint1 hold;
  uint1 calendar;
  uint1 irqflag;
  uint1 roundseconds;

  uint1 irqmask;
  uint1 irqduty;
  uint2 irqperiod;

  uint1 pause;
  uint1 stop;
  uint1 atime;  //astronomical time (24-hour mode)
  uint1 test;

  //memory.cpp
  auto rtc_reset() -> void;
  auto rtc_read(uint4 addr) -> uint4;
  auto rtc_write(uint4 addr, uint4 data) -> void;

  auto load(const uint8* data) -> void;
  auto save(uint8* data) -> void;

  //time.cpp
  auto irq(uint2 period) -> void;
  auto duty() -> void;
  auto round_seconds() -> void;
  auto tick() -> void;

  auto tick_second() -> void;
  auto tick_minute() -> void;
  auto tick_hour() -> void;
  auto tick_day() -> void;
  auto tick_month() -> void;
  auto tick_year() -> void;
};

extern EpsonRTC epsonrtc;
