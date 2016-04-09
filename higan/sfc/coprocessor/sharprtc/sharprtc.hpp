struct SharpRTC : Cothread {
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

  enum class State : uint { Ready, Command, Read, Write } rtc_state;
  int rtc_index;

  uint second;
  uint minute;
  uint hour;
  uint day;
  uint month;
  uint year;
  uint weekday;

  //memory.cpp
  auto rtc_read(uint4 addr) -> uint4;
  auto rtc_write(uint4 addr, uint4 data) -> void;

  auto load(const uint8* data) -> void;
  auto save(uint8* data) -> void;

  //time.cpp
  static const uint daysinmonth[12];
  auto tick_second() -> void;
  auto tick_minute() -> void;
  auto tick_hour() -> void;
  auto tick_day() -> void;
  auto tick_month() -> void;
  auto tick_year() -> void;

  auto calculate_weekday(uint year, uint month, uint day) -> uint;
};

extern SharpRTC sharprtc;
